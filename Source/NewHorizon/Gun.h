// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class NEWHORIZON_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void shoot();
private:
	bool gunTrace(FHitResult& hitResult, FVector& shotDirection);

	AController* getOwnerController() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere) USceneComponent* root;
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* mesh;
private:
	UPROPERTY(EditAnywhere) UParticleSystem* muzzleFlash;
	UPROPERTY(EditAnywhere) USoundBase* muzzleSound;
	UPROPERTY(EditAnywhere) UParticleSystem* impactParticles;
	UPROPERTY(EditAnywhere) USoundBase* impactSound;
	UPROPERTY(EditAnywhere) float maxRange = 50000;
	UPROPERTY(EditAnywhere) float damage = 10.0f;
};
