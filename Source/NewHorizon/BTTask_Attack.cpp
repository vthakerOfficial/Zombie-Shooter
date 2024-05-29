// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MyCharacter.h"
#include "AIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) {
		return EBTNodeResult::Failed;
	}
	AMyCharacter* playerCharacter = Cast<AMyCharacter>(AIController->GetPawn());
	if (!playerCharacter) {
		return EBTNodeResult::Failed;
	}

	playerCharacter->lunge();
	return EBTNodeResult::Succeeded;
}
