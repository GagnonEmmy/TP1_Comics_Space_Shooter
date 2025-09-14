// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Projectile.h"
#include "SpaceShip.generated.h"

UCLASS()
class COMICS_SPACE_SHOOTER_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* MuzzleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* IA_Fire;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxSpeed = 900.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationInterpSpeed = 12.f;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AProjectile> ProjectileClass; //Permet de stocker une référence à une classe et non à une instance

	UPROPERTY(EditAnywhere, Category="Projectile")
	float FireRate = 0.2f;
	
	float InitialZ;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	float LastFireTime = -1.f;
};
