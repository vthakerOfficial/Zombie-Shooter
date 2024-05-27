// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
//#include "InputDataConfig.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gun.h"
#include "HorizonGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include <GameFramework/SpringArmComponent.h>

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* playerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			subsystem->AddMappingContext(inputMappingContext, 0);
		}
	}
	// setting up player camera
	springArmComponent = GetComponentByClass<USpringArmComponent>();
	if (cameraFloatCurve && springArmComponent) {
		if (APlayerController* playerController = GetWorld()->GetFirstPlayerController()) {
			if (APlayerCameraManager* cameraManager = playerController->PlayerCameraManager) {
				cameraManager->ViewPitchMin = -80;
				cameraManager->ViewPitchMax = 90;
				cameraTimeline = FTimeline();
				FOnTimelineFloatStatic timelineCallback;
				timelineCallback.BindUFunction(this, FName("moveCameraTimelineCallback"));
				// need cameraFloatCurve
				cameraTimeline.AddInterpFloat(cameraFloatCurve, timelineCallback);
				// need nth
				defaultArmLength = springArmComponent->TargetArmLength;
				defaultSocketYOffset = springArmComponent->SocketOffset.Y;
			}
		}
	}
	bIsSprinting = false;
	sprintMultiplier = 10.0f;

	//attaching gun
	isPlayer = Cast<APlayerController>(GetController()) != nullptr;
	if (isPlayer) // is player controlled
	{
		gun = GetWorld()->SpawnActor<AGun>(gunClass);
		gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponSocket"));
		gun->SetOwner(this);
	}

	// setting up health
	health = maxHealth;
}

void AMyCharacter::moveCamera(float newTargetArmLength, float newTargetSocketYOffset) {
	if (!springArmComponent || !cameraFloatCurve) return;
	targetArmLength = newTargetArmLength; targetSocketYOffset = newTargetSocketYOffset;
	stopCameraMovement();
	lastSocketYOffset = springArmComponent->SocketOffset.Y;
	lastArmLength = springArmComponent->TargetArmLength;
	cameraTimeline.PlayFromStart();
}

void AMyCharacter::stopCameraMovement()
{
	cameraTimeline.Stop();
}


void AMyCharacter::resetCameraPosition()
{
	//if (!springArmComponent || !cameraFloatCurve) return;
	UE_LOG(LogTemp, Display, TEXT("Camera Reset"));
	moveCamera(defaultArmLength, defaultSocketYOffset);
}

void AMyCharacter::moveCameraTimelineCallback(float value)
{
	if (!springArmComponent) return;
	float newTargetArmLength = FMath::Lerp(lastArmLength, targetArmLength, value);
	springArmComponent->TargetArmLength = newTargetArmLength;
	float newSocketYOffset = FMath::Lerp(lastSocketYOffset, targetSocketYOffset, value);
	springArmComponent->SocketOffset.Y = newSocketYOffset;
}

bool AMyCharacter::isSprinting() const
{
	return bIsSprinting;
}



// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cameraTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		enhancedInput->BindAction(moveAction, ETriggerEvent::Triggered, this, &AMyCharacter::move);
		enhancedInput->BindAction(lookAction, ETriggerEvent::Triggered, this, &AMyCharacter::look);
		enhancedInput->BindAction(sprintAction, ETriggerEvent::Started, this, &AMyCharacter::sprintStart);
		enhancedInput->BindAction(sprintAction, ETriggerEvent::Completed, this, &AMyCharacter::sprintStop);
		enhancedInput->BindAction(jumpAction, ETriggerEvent::Started, this, &AMyCharacter::doubleJump);
		enhancedInput->BindAction(crouchAction, ETriggerEvent::Started, this, &AMyCharacter::crouchStart);
		enhancedInput->BindAction(crouchAction, ETriggerEvent::Completed, this, &AMyCharacter::crouchStop);
		enhancedInput->BindAction(shootAction, ETriggerEvent::Started, this, &AMyCharacter::shoot);
	}
}

void AMyCharacter::Landed(const FHitResult& hit)
{
	Super::Landed(hit);
	bIsFirstJump = true;
	bCanDoubleJump = false;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageToApply = FMath::Min(DamageAmount, health);
	health -= damageToApply;
	if (isDead()) {
		//GetMesh()->SetSimulatePhysics(true);
		if (AHorizonGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AHorizonGameModeBase>()) { // telling gamemode that we died
			gameMode->pawnKilled(this);
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), deathParticles, GetActorTransform());
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AMyCharacter::destroySelf, 3.0f, false);
		if (!isPlayer) {
			Destroy();
		}
		//if (isPlayer) {
		//	UE_LOG(LogTemp, Display, TEXT("Player died--------------------------------------------------------------"));
		//}
		//else { // is ai
		//	//GetCharacterMovement()->DisableMovement();
		//	UE_LOG(LogTemp, Display, TEXT("ai died-------------------------------------------------"));
		//}
	}

	return damageToApply;
}

bool AMyCharacter::isDead() const
{
	return health == 0;
}


void AMyCharacter::move(const FInputActionValue& value)
{
	const FVector2d moveVector = value.Get<FVector2d>();


	FRotator curRotation = Controller->GetControlRotation();
	FRotator yawRotation(0.f, curRotation.Yaw, 0.f);

	FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDirection, moveVector.Y);
	FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDirection, moveVector.X);
}
void AMyCharacter::look(const FInputActionValue& value) {
	//UE_LOG(LogTemp, Display, TEXT("Moving camera"));
	FVector2d rotationVector = value.Get<FVector2d>();
	AddControllerYawInput(rotationVector.X);
	AddControllerPitchInput(rotationVector.Y);
	//UE_LOG(LogTemp, Display, TEXT("rotationVectr: (%f, %f)"), rotationVector.X, rotationVector.Y);
}

void AMyCharacter::sprintStart(const FInputActionValue& value)
{
	crouchStop();
	bIsSprinting = true;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed *= sprintMultiplier;
	moveCamera(defaultArmLength*2, 20);
}

void AMyCharacter::sprintStop(const FInputActionValue& value)
{
	bIsSprinting = false;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed /= sprintMultiplier;
	resetCameraPosition();
}

void AMyCharacter::doubleJump() {
	if (bIsFirstJump && GetCharacterMovement()->IsMovingOnGround()) {
		ACharacter::Jump();
		bIsFirstJump = false;
		bCanDoubleJump = true;
		UE_LOG(LogTemp, Display, TEXT("nOrmal jump"));
	}
	else if (bCanDoubleJump){ // double jump time!
		FVector forwardDir = GetActorRotation().Vector();
		LaunchCharacter(FVector(forwardDir.X * 6000, forwardDir.Y * 6000, 1500), true, true);
		bCanDoubleJump = false;
		UE_LOG(LogTemp, Display, TEXT("double jump"));
	}
}

void AMyCharacter::crouchStart() {
	if (bIsCrouching) return;
	if (UCharacterMovementComponent* moveComp = Cast<UCharacterMovementComponent>(GetMovementComponent())) {
		moveComp->MaxWalkSpeed /= 2;
	}
	auto capsuleComp = GetCapsuleComponent();
	//capsuleComp->SetCapsuleHalfHeight(capsuleComp->GetUnscaledCapsuleHalfHeight() / 2);
	ACharacter::Crouch();
	bIsCrouching = true;
}
void AMyCharacter::crouchStop() {
	if (!bIsCrouching) return;
	if (UCharacterMovementComponent* moveComp = Cast<UCharacterMovementComponent>(GetMovementComponent())) {
		moveComp->MaxWalkSpeed *= 2;
	}
	auto capsuleComp = GetCapsuleComponent();
	//capsuleComp->SetCapsuleHalfHeight(capsuleComp->GetUnscaledCapsuleHalfHeight() * 2);
	ACharacter::UnCrouch();
	bIsCrouching = false;
}

void AMyCharacter::destroySelf()
{
	Destroy();
}

void AMyCharacter::shoot()
{
	gun->shoot();
}

void AMyCharacter::lunge() {

}

float AMyCharacter::getHealthAsPercent() const
{
	return health / maxHealth;
}


