// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorizonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEWHORIZON_API AHorizonPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	
private:
	UPROPERTY(EditAnywhere)
	float restartDelay = 3;
	FTimerHandle restartTimerHandle;
	UPROPERTY(EditAnywhere) TSubclassOf<class UUserWidget> scoreScreenClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UUserWidget> gameOverScreenClass;

	FTimerHandle removeTempWidgetHandle;
	UUserWidget* tempScreen;
	void removeTempWidget();
};
