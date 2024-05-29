// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class NEWHORIZON_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure) bool isDead() const;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere) class UBehaviorTree* AIBehavior;

	APawn* playerPawn;

	bool bTargetPlayer;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector buildingLocation;
};
