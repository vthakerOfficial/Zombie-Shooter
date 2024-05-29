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

	virtual void Landed(const FHitResult& hit);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintPure) bool isDead() const;
	void shoot();
	void lunge();
	UFUNCTION(BlueprintPure) float getHealthAsPercent() const;
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
	void doubleJump();
	void crouchStart();
	void crouchStop();
	void destroySelf();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputMappingContext* inputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* lookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* sprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* jumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* crouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* shootAction;

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
	
	// double jump logic
	bool bIsFirstJump = true;
	bool bCanDoubleJump = false;

	// crouch logic (not needed bcuz move comp alr got it)
	UPROPERTY(BlueprintReadOnly) bool bIsCrouching = false; 


	// attaching gun 
	UPROPERTY(EditDefaultsOnly, Category = "Combat") TSubclassOf<class AGun> gunClass;

	UPROPERTY() class AGun* gun;

	// setting up health
	UPROPERTY(EditDefaultsOnly, Category = "Combat") float maxHealth = 100;
	float health;

	// death timer handle
	FTimerHandle deathTimerHandle;

	// particles
	UPROPERTY(EditAnywhere) UParticleSystem* deathParticles;
	//UPROPERTY(EditAnywhere) USoundBase* attack;

private:
	bool isPlayer;
};

