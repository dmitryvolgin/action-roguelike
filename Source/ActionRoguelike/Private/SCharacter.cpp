// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	AddMovementInput(ControlRotation.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	const FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);
	GetWorldTimerManager().SetTimer(PrimaryAttackTimerHandle, this, &ASCharacter::OnPrimaryAttackReady, 0.17f);
}

void ASCharacter::OnPrimaryAttackReady()
{
	if (!ensure(ProjectileClass))
	{
		return;
	}
	
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	FVector TraceStartLocation = CameraComp->GetComponentLocation();
	FVector TraceEndLocation = TraceStartLocation + CameraComp->GetForwardVector() * 10000;

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FHitResult HitResult;

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStartLocation, TraceEndLocation, QueryParams);
	FVector TargetLocation = bHit ? HitResult.Location : TraceEndLocation;

	FRotator ProjectileRotation = (TargetLocation - HandLocation).Rotation();
	
	const FTransform SpawnTransform = FTransform(ProjectileRotation, HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}