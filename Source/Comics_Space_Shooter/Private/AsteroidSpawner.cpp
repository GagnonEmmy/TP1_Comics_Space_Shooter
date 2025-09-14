// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

#include "Asteroid.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnDistanceMin = 15000 / 2 + 1000;
	SpawnDistanceMax = SpawnDistanceMin + 2000;
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		TargetActor = PC->GetPawn();
	}
	
	// Lancer le timer
	float InitialDelay = FMath::RandRange(SpawnMinTime, SpawnMaxTime);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AAsteroidSpawner::SpawnAsteroid, InitialDelay, false);
}

// Called every frame
void AAsteroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsteroidSpawner::SpawnAsteroid()
{
	if(!AsteroidClass || !TargetActor) return;

	FVector PlayerLocation = TargetActor->GetActorLocation();

	// Spawn hors écran
	float Angle = FMath::RandRange(0.f, 360.f);
	float Distance = FMath::RandRange(SpawnDistanceMin, SpawnDistanceMax);

	float SpawnX = PlayerLocation.X + FMath::Cos(FMath::DegreesToRadians(Angle)) * Distance;
	float SpawnY = PlayerLocation.Y + FMath::Sin(FMath::DegreesToRadians(Angle)) * Distance;
	float SpawnZ = 1000.f;

	FVector SpawnLocation(SpawnX, SpawnY, SpawnZ);

	FRotator SpawnRotation = FRotator::ZeroRotator;

	AAsteroid* Asteroid = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass, SpawnLocation, SpawnRotation);
	if(Asteroid)
	{
		Asteroid->Target = TargetActor;
	}

	// Relancer le timer pour spawn suivant
	float NextTime = FMath::RandRange(SpawnMinTime, SpawnMaxTime);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AAsteroidSpawner::SpawnAsteroid, NextTime, false);
}

