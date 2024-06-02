// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class NEWHORIZON_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;


public:
	void spawnEnemy(int32 amt);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning") TSubclassOf<class AMyCharacter> enemyClass;
	UPROPERTY(EditAnywhere, Category = "Spawning") double spawnRadius = 5000;
};
