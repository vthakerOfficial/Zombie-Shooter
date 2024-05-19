// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InputDataConfig.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>
#include "GameFramework/CharacterMovementComponent.h"
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
	}
}


void AMyCharacter::move(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Display, TEXT("Moving yawn"));
	const FVector2d moveVector = value.Get<FVector2d>();

	//UPawnMovementComponent* comp = GetMovementComponent();

	FRotator curRotation = Controller->GetControlRotation();
	FRotator yawRotation(0.f, curRotation.Yaw, 0.f);

	FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDirection, moveVector.Y);
	FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDirection, moveVector.X);
	//UE_LOG(LogTemp, Display, TEXT("Player Velocity: %s"), GetVelocity().ToString());
	//if (moveVector.GetAbs().X > .05f) { // avoid accidental movement on faulty controller

	//	AddMovementInput(GetActorForwardVector() * moveVector.X);
	//}
	//// if movement is 
	//if (moveVector.GetAbs().Y > .05f) {
	//	//const FVector directionVector = moveRotation.RotateVector(FVector::ForwardVector);
	//	//UE_LOG(LogTemp, Display, TEXT("C"));
	//	//AddMovementInput()
	//	AddMovementInput(GetActorRightVector() * moveVector.Y);
	//}
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
	bIsSprinting = true;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed *= sprintMultiplier;
	moveCamera(200, 20);
}

void AMyCharacter::sprintStop(const FInputActionValue& value)
{
	bIsSprinting = false;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed /= sprintMultiplier;
	resetCameraPosition();
}

void AMyCharacter::moveRight(float axisVal)
{
	AddMovementInput(GetActorRightVector() * axisVal);
}
void AMyCharacter::lookUp(float axisVal)
{
	AddControllerPitchInput(axisVal);
}

void AMyCharacter::lookRight(float axisVal)
{
	AddControllerYawInput(axisVal);
}
void AMyCharacter::jump() {
	//jump();
}



