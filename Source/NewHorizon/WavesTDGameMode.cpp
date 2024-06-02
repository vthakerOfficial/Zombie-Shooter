// Fill out your copyright notice in the Description page of Project Settings.


#include "WavesTDGameMode.h"
#include "EngineUtils.h"
#include "ZombieAIController.h"
#include <Misc/OutputDeviceNull.h>
#include "ZombieSpawner.h"
#include "MyCharacter.h"

void AWavesTDGameMode::pawnKilled(APawn* pawnKilled)
{
	Super::pawnKilled(pawnKilled);

	if (APlayerController* playerController = Cast<APlayerController>(pawnKilled->GetController())) {
		endGame(false);
	}
	else {
		for (AZombieAIController* controller : TActorRange<AZombieAIController>(GetWorld())) {
			if (!controller->isDead()) return;
		}
		numRoundsSurvived++;
		for (TActorIterator<AMyCharacter> it(GetWorld()); it; ++it) {
			AMyCharacter* player = *it;
			if (!it) continue;
			//UE_LOG(LogTemp, Display, TEXT("FOUND EM"));
			it->resetHealth();
			//break;
		}
		endGame(true);
		GetWorldTimerManager().SetTimer(spawnZombiesTimerHandle, this, &AWavesTDGameMode::spawnZombies, 4.0f);
		
	}
	//UE_LOG(LogTemp, Display, TEXT("A Pawn Was Killed!--------------------------------------------------------"));
}

int AWavesTDGameMode::roundsSurvived() const
{
	return numRoundsSurvived;
}

void AWavesTDGameMode::StartPlay()
{
	Super::StartPlay();
	spawnZombies();
}


void AWavesTDGameMode::endGame(bool bIsPlayerWinner)
{
	for (AController* controller : TActorRange<AController>(GetWorld())) {
		bool bIsWinner = controller->IsPlayerController() == bIsPlayerWinner;
		controller->GameHasEnded(controller->GetPawn(), bIsWinner);
	}
}



void AWavesTDGameMode::spawnZombies() {
	//UE_LOG(LogTemp, Display, TEXT("Spawning zombies"));
	//if (spawnZombiesBPClass) {
	//	UE_LOG(LogTemp, Display, TEXT("Was able to spawn zombies"));
	//	const FString cmd = FString::Printf(TEXT("Spawn Zombies %d"), );
	//	//spawnZombiesBPClass->CallFunctionByNameWithArguments(*cmd, ar, NULL, true);
	//}
	//UE_LOG(LogTemp, Display, TEXT(""));
	//int32 numZombies = 2.0f * FMath::Pow(2.0f, numRoundsSurvived);
	//FOutputDeviceNull ar;
	//CallFunctionByNameWithArguments(TEXT("MySpawnZombies 5"), ar, NULL, true);
	//UE_LOG(LogTemp, Display, TEXT("Spawning"));
	for (TActorIterator<AZombieSpawner> it(GetWorld()); it; ++it) {
		AZombieSpawner* spawner = *it;
		if (!it) continue;
		//UE_LOG(LogTemp, Display, TEXT("FOUND EM"));
		it->spawnEnemy(2.0f * FMath::Pow(spawnMultiplier, numRoundsSurvived));
		//break;
	}
	/*if (AActor* foundActor = UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<AZombieSpawner>())) {
		if (AZombieSpawner* spawner = Cast<AZombieSpawner>(foundActor)) {
			spawner->spawnEnemy(2.0f * FMath::Pow(2.0f, numRoundsSurvived));
		}
	}*/
}


