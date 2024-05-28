// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizonPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AHorizonPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	//UE_LOG(LogTemp, Display, TEXT("GameHasEnded has been called"));
	
	if (bIsWinner) {
		tempScreen = CreateWidget(this, scoreScreenClass);
		GetWorldTimerManager().SetTimer(removeWidgetTimerHandle, this, &AHorizonPlayerController::removeWidget, restartDelay);
	}
	else {
		tempScreen = CreateWidget(this, gameOverScreenClass);
		GetWorldTimerManager().SetTimer(restartTimerHandle, this, &APlayerController::RestartLevel, restartDelay * 2.0f);
	}
	if (tempScreen) {
		tempScreen->AddToViewport();
	}

}

void AHorizonPlayerController::removeWidget()
{
	if (tempScreen) {
		tempScreen->RemoveFromViewport();
	}
}
