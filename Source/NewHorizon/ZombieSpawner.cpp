// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawner.h"
#include "MyCharacter.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AZombieSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AZombieSpawner::spawnEnemy(int32 amt)
{
	UWorld* world = GetWorld();
	for (int32 i = 0; i < amt; i++) {
		AMyCharacter* enemy = world->SpawnActor<AMyCharacter>(enemyClass, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	}
}

