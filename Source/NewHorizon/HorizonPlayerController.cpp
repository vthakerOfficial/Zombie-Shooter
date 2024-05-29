// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizonPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AHorizonPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	//UE_LOG(LogTemp, Display, TEXT("GameHasEnded has been called"));
	UUserWidget* screen;
	if (bIsWinner) {
		screen = CreateWidget(this, scoreScreenClass);
	}
	else {
		screen = CreateWidget(this, gameOverScreenClass);
	}
	if (screen) {
		screen->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(restartTimerHandle, this, &APlayerController::RestartLevel, restartDelay * 2.0f);
}
