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
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	FTimerHandle TravelTimerHandle;

	UPROPERTY(EditAnywhere)
	float TravelTime;

	UPROPERTY(EditAnywhere)
	float DetonationTime;
};