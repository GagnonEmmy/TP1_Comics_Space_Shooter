// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionBoom.generated.h"

UCLASS()
class COMICS_SPACE_SHOOTER_API AExplosionBoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosionBoom();

	// Meshs alternatifs
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	TArray<UMaterial*> MaterialVariants;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
