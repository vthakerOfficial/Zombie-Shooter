// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrainComponent.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
	NodeName = TEXT("Move To Target");
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (APawn* pawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn())) {
		if (AAIController* AIController = Cast<AAIController>(pawn->GetController())) {
			//auto a = BlackboardKey.GetSelectedKeyID()

			FVector targetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
			EPathFollowingRequestResult::Type moveResult = (AIController->MoveToLocation(targetLocation, 100.0f));
			if (moveResult == 2) { //2 is requestSuccesful of EPathFollowingRequestResult::Type:: enum
				return EBTNodeResult::Succeeded;
			}
			else {
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Failed;
}
