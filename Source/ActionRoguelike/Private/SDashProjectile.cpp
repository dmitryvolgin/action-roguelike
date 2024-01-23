// Fill out your copyright notice in the Description page of Project Settings.

#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	MovementComponent->InitialSpeed = 6000.0f;
	TravelTime = 0.2f;
	DetonationTime = 0.2f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TravelTimerHandle, this, &ASDashProjectile::Explode, TravelTime);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TravelTimerHandle);

	MovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	// Creating timer handle here bc we do not need to have it saved globally for clearing or so on.
	FTimerHandle DelayedTeleportTimerHandle;
	GetWorldTimerManager().SetTimer(DelayedTeleportTimerHandle, this, &ASDashProjectile::TeleportInstigator, DetonationTime);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleEffect, GetActorLocation());
	EffectComponent->DeactivateSystem();
	
	// Skip base implementation for projectile to live a little longer.
	//Super::Explode_Implementation();
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, true);
	}
	
	Destroy();
}