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
	}
	else {
		tempScreen = CreateWidget(this, gameOverScreenClass);
	}
	if (tempScreen) {
		tempScreen->AddToViewport();
	}

	if (!bIsWinner) {
		GetWorldTimerManager().SetTimer(restartTimerHandle, this, &APlayerController::RestartLevel, restartDelay * 2.0f);
	}
	else {

		GetWorldTimerManager().SetTimer(removeTempWidgetHandle, this, &AHorizonPlayerController::removeTempWidget, restartDelay);
	}
}

void AHorizonPlayerController::removeTempWidget()
{
	if (tempScreen) {
		tempScreen->RemoveFromViewport();
	}
}
