// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"
#include "EnhancedInputComponent.h"
#include "Projectile.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);

	// Réglage du rayon de la capsule (selon la taille de ton vaisseau)
	CollisionComponent->InitSphereRadius(50.f);

	// Configure les collisions pour détecter les objets (astéroïdes)
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Activation des collisions
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Canal dédié pour le vaisseau
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Ignore toutes les collisions sauf celle avec l'astéroïde
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore); // Ignore les projectiles (tu peux les gérer séparément)
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap); // Blocke les collisions avec des objets dynamiques comme l'astéroïde

	MuzzleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
	MuzzleComponent->SetupAttachment(StaticMesh);
	MuzzleComponent->SetRelativeLocation(FVector(0, 340, 0));

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = MaxSpeed;

	//Empêcher le mouvements en Z
	MovementComponent->SetPlaneConstraintEnabled(true);
	MovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	InitialZ = GetActorLocation().Z;
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Garder le vaisseau sur le Z d'origine
	FVector Loc = GetActorLocation();
	SetActorLocation(FVector(Loc.X, Loc.Y, InitialZ));
	
	//Regarder la souris
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector WorldOrigin, WorldDirection;
		if (PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
		{
			float T = (InitialZ - WorldOrigin.Z) / WorldDirection.Z;
			if (T > 0.f)
			{
				FVector MouseWorldPos = WorldOrigin + WorldDirection * T;
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MouseWorldPos);
				FRotator TargetRot(0.f, LookAt.Yaw -90.0f, 0.f);
				FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotationInterpSpeed);
				SetActorRotation(NewRot);
			}
		}
	}
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)
		{
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASpaceShip::Move);
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Completed, this, &ASpaceShip::Move);
		}

		if (IA_Fire)
		{
			EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ASpaceShip::Fire);
		}
	}
}

void ASpaceShip::Move(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();

	AddMovementInput(FVector::ForwardVector, Axis.Y); // W/S
	AddMovementInput(FVector::RightVector, Axis.X);   // A/D
}

void ASpaceShip::Fire(const FInputActionValue& Value)
{
	if (!ProjectileClass) return;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < FireRate) return; // cooldown
	LastFireTime = CurrentTime;

	// Position et rotation depuis le Muzzle
	FVector SpawnLocation = MuzzleComponent->GetComponentLocation() + MuzzleComponent->GetForwardVector() * 10.f;
	FRotator SpawnRotation = MuzzleComponent->GetComponentRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();

	// Spawn du projectile
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParameters
	);
}
