// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	ASDashProjectile();
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	void OnTravelTimerEnd();
	void OnDetonationTimerEnd();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle TravelTimerHandle;
	FTimerHandle DetonationTimerHandle;

	UPROPERTY(EditAnywhere)
	float TravelTime;

	UPROPERTY(EditAnywhere)
	float DetonationTime;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DetonationParticleSystem;
};
