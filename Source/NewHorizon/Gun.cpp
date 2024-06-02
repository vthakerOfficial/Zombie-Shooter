// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "MyCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::shoot() {
	UGameplayStatics::SpawnEmitterAttached(muzzleFlash, mesh, TEXT("muzzleFlashSocket"));

	FHitResult hitResult;
	FVector shotDirection;
	bool bHitSomething = gunTrace(hitResult, shotDirection);
	if (bHitSomething)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactParticles, hitResult.Location, shotDirection.Rotation());

		AActor* hitActor = hitResult.GetActor();
		if (hitActor)
		{
			if (Cast<AMyCharacter>(hitActor)) { // if zombie
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, hitResult.Location, shotDirection.Rotation());
			}
			else {

				UGameplayStatics::SpawnSoundAttached(muzzleSound, mesh, TEXT("muzzleFlashSocket"));
			}
			FPointDamageEvent damageEvent(damage, hitResult, shotDirection, nullptr);
			hitActor->TakeDamage(damage, damageEvent, getOwnerController(), this);
		}
	}
	else {
		UGameplayStatics::SpawnSoundAttached(muzzleSound, mesh, TEXT("muzzleFlashSocket"));
	}
}

bool AGun::gunTrace(FHitResult& hitResult, FVector& shotDirection)
{
	AController* ownerController = getOwnerController();
	if (!ownerController) return false;

	FVector camLocation;
	FRotator camRotation;
	ownerController->GetPlayerViewPoint(camLocation, camRotation);
	shotDirection = -camRotation.Vector();

	FCollisionQueryParams collisionIgnoreList;
	collisionIgnoreList.AddIgnoredActor(this);
	collisionIgnoreList.AddIgnoredActor(GetOwner());

	FVector endLocation = camLocation + camRotation.Vector() * maxRange;
	return GetWorld()->LineTraceSingleByChannel(hitResult, camLocation, endLocation, ECollisionChannel::ECC_GameTraceChannel1, collisionIgnoreList);
}
AController* AGun::getOwnerController() const
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());  // dont need to typecast to ashooter character bcuz just need the APawn aspect of it
	if (!ownerPawn) return nullptr;

	return ownerPawn->GetController();
}
// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

