// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Health = 100.0f;
}

bool USAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	Health += DeltaHealth;
	return true;
}