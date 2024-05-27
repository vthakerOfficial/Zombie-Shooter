// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizonPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AHorizonPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	UE_LOG(LogTemp, Display, TEXT("Game Over"));

	UUserWidget* scoreScreen = CreateWidget(this, scoreScreenClass);
	if (scoreScreen) {
		scoreScreen->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(restartTimerHandle, this, &APlayerController::RestartLevel, restartDelay);
}
