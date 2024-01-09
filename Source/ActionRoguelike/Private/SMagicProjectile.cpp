// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	MovementComponent->InitialSpeed = 1000.0f;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
}