// Fill out your copyright notice in the Description page of Project Settings.

#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASDashProjectile::ASDashProjectile()
{
	MovementComponent->InitialSpeed = 5000.0f;
	TravelTime = 0.2f;
	DetonationTime = 0.2f;
	DetonationParticleSystem = nullptr;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TravelTimerHandle, this, &ASDashProjectile::OnTravelTimerEnd, TravelTime);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnHit);
}

void ASDashProjectile::OnTravelTimerEnd()
{
	MovementComponent->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DetonationParticleSystem, GetActorLocation());
	GetWorldTimerManager().SetTimer(DetonationTimerHandle, this, &ASDashProjectile::OnDetonationTimerEnd, DetonationTime);
}

void ASDashProjectile::OnDetonationTimerEnd()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation(), false, true);
	Destroy();
}

void ASDashProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(TravelTimerHandle);
	TravelTimerHandle.Invalidate();
	OnTravelTimerEnd();
}
