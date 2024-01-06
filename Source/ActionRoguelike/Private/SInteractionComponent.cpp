// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "Tests/AutomationEditorCommon.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* OwnerActor = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	OwnerActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, QueryParams);

	float Raduis = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Raduis);

	FColor DebugColor = FColor::Red;
	
	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, QueryParams, Shape);

	for (FHitResult Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(OwnerActor);
				ISGameplayInterface::Execute_Interact(HitActor, OwnerPawn);

				DebugColor = FColor::Green;
				DrawDebugSphere(GetWorld(), Hit.Location, Raduis, 16, DebugColor, false, 5.0f);

				break;
			}
		}
	}
	
	DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 5.0f, 0, 2.0f);
}