// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

bool AZombieAIController::isDead() const
{
	AMyCharacter* controlledCharacter = Cast<AMyCharacter>(GetPawn());
	if (controlledCharacter) {
		return controlledCharacter->isDead();
	}
	return true;
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);
		playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	//bTargetPlayer = UKismetMathLibrary::RandomBoolWithWeight(.5f);
	//UE_LOG(LogTemp, Display, TEXT("should target player? %d"), bTargetPlayer);
	bTargetPlayer = true;
}

void AZombieAIController::Tick(float DeltaTime)
{
	if (bTargetPlayer) {
		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), playerPawn->GetActorLocation()); //GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}
	else {
		// target building
		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), buildingLocation);
	}
}
