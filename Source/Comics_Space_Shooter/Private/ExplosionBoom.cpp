// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionBoom.h"

// Sets default values
AExplosionBoom::AExplosionBoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExplosionBoom::BeginPlay()
{
	Super::BeginPlay();

	Mesh = Cast<UStaticMeshComponent>(RootComponent->GetAttachChildren()[0]);

	// Choisir un material aléatoire
	if(MaterialVariants.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, MaterialVariants.Num() - 1);
		Mesh->SetMaterial(0, MaterialVariants[Index]);
	}

	SetLifeSpan(2.f);
}

// Called every frame
void AExplosionBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

