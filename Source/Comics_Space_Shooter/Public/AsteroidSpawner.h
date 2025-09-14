// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

UCLASS()
class COMICS_SPACE_SHOOTER_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidSpawner();

	// Zone de spawn autour du joueur
	UPROPERTY(EditAnywhere)
	float SpawnDistanceMin = 15000.f;
	UPROPERTY(EditAnywhere)
	float SpawnDistanceMax = 3000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAsteroid> AsteroidClass;

	// Spawn time
	UPROPERTY(EditAnywhere)
	float SpawnMinTime = 2.f;
	UPROPERTY(EditAnywhere)
	float SpawnMaxTime = 5.f;
	
	FTimerHandle SpawnTimerHandle; //Spawn Rate

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnAsteroid();

};
