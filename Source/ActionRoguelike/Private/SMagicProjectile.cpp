// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	MovementComponent->InitialSpeed = 1000.0f;
}