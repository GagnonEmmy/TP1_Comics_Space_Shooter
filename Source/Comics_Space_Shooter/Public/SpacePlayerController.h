// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpacePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COMICS_SPACE_SHOOTER_API ASpacePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category="Input")
	class UInputMappingContext* IMC_SpaceShooter;
};
