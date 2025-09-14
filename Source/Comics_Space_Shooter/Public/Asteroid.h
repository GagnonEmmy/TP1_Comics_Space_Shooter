// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplosionBoom.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

UCLASS()
class COMICS_SPACE_SHOOTER_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float Speed = 300.f;

	//Référence vers l'acteur du vaisseau
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	AActor* Target;

	// Meshs alternatifs
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	TArray<UStaticMesh*> MeshVariants;

	UPROPERTY(EditAnywhere, Category="Explosion")
	TSubclassOf<AExplosionBoom> ExplosionClass;

	UPROPERTY(EditAnywhere, Category = "Health")
	float Life;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxLife;

	UPROPERTY(EditAnywhere, Category = "Health")
	UWidgetComponent* HealthWidget;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyAsteroid();

	void UpdateHealthBar();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
