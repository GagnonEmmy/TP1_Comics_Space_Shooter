// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Asteroid.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true; //Pour qu'il se dirige dans la direction ou il est lancé et qu'il fasse face à cette direction
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bSweepCollision = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	UStaticMeshComponent* ChildMesh = Cast<UStaticMeshComponent>(RootComponent->GetAttachChildren()[0]);

	ChildMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ChildMesh->SetCollisionObjectType(ECC_GameTraceChannel1); // correspond à celui défini pour les astéroïdes
	ChildMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ChildMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 100.f; // distance à parcourir cette frame

	FHitResult HitResult;
	if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(10.f)))
	{
		// détruire le projectile et l'astéroïde
		if (AActor* HitActor = HitResult.GetActor())
		{
			AAsteroid* HitAsteroid = Cast<AAsteroid>(HitActor);
			if (HitAsteroid) // ✅ Vérifie si le cast a réussi
			{
				HitAsteroid->DestroyAsteroid();
			}
		}
		Destroy();
	}

	// Avancer le projectile
	SetActorLocation(End);
}

