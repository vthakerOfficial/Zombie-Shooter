// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InputDataConfig.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>

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


}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		enhancedInput->BindAction(moveAction, ETriggerEvent::Triggered, this, &AMyCharacter::move);
		UE_LOG(LogTemp, Display, TEXT("Binded"));
		enhancedInput->BindAction(lookAction, ETriggerEvent::Triggered, this, &AMyCharacter::look);
	}


	
}


void AMyCharacter::move(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Display, TEXT("Shmoovinggg"));
	const FVector2d moveVector = value.Get<FVector2d>();
	
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


	FRotator curRotation = Controller->GetControlRotation();
	FRotator yawRotation(0.f, curRotation.Yaw, 0.f);

	FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDirection, moveVector.Y);
	FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDirection, moveVector.X);
}
void AMyCharacter::look(const FInputActionValue& value) {
	UE_LOG(LogTemp, Display, TEXT("Moving camera"));
	FVector2d rotationVector = value.Get<FVector2d>();
	AddControllerYawInput(rotationVector.X);
	AddControllerPitchInput(rotationVector.Y);
	UE_LOG(LogTemp, Display, TEXT("rotationVectr: (%f, %f)"), rotationVector.X, rotationVector.Y);
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



