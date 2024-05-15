// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataConfig.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class NEWHORIZON_API UInputDataConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* moveForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* moveRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* lookUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* lookRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* jump;
};
