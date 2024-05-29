// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HorizonGameModeBase.h"
#include "WavesTDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEWHORIZON_API AWavesTDGameMode : public AHorizonGameModeBase
{
	GENERATED_BODY()
public:
	virtual void pawnKilled(APawn* pawnKilled) override;
	UFUNCTION(BlueprintPure) int roundsSurvived() const;
	virtual void StartPlay();



private:
	void endGame(bool bIsPlayerWinner);
	void spawnZombies();
private:
	int numRoundsSurvived = 0;

	UPROPERTY(EditDefaultsOnly) TSubclassOf<class AMyCharacter> zombieClass;

	UPROPERTY(EditAnywhere, Category = "Blueprint Class") TSubclassOf<AActor> spawnZombiesBPClass;

	FTimerHandle spawnZombiesTimerHandle;
	
};
