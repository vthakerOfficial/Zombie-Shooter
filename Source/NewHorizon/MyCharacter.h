// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include <Components/TimelineComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacter.generated.h"

class UInputAction;
UCLASS()
class NEWHORIZON_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	void moveCamera(float targetArmLength, float targetSocketYOffset);
	void stopCameraMovement();	
	void resetCameraPosition();
	UFUNCTION()
	void moveCameraTimelineCallback(float value);

	UFUNCTION(BlueprintPure)
	bool isSprinting() const;

private:
	void move(const FInputActionValue& value);
	void look(const FInputActionValue& value);
	void sprintStart(const FInputActionValue& value);
	void sprintStop(const FInputActionValue& value);
	void moveRight(float axisVal);
	void lookUp(float axisVal);
	void lookRight(float axisVal);
	void jump();

protected:/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnhancedInput")
	class UInputDataConfig* inputActions;*/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputMappingContext* inputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* lookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* sprintAction;

	// camera logic
	USpringArmComponent* springArmComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCurveFloat* cameraFloatCurve;
	FTimeline cameraTimeline;
	float defaultArmLength;
	float defaultSocketYOffset;
	float targetArmLength;
	float targetSocketYOffset;
	float lastArmLength;
	float lastSocketYOffset;

	// sprint logic
	bool bIsSprinting;
	float sprintMultiplier;


};

