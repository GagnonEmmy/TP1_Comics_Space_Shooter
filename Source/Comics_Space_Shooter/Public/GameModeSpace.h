// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeSpace.generated.h"

/**
 * 
 */
UCLASS()
class COMICS_SPACE_SHOOTER_API AGameModeSpace : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* ScoreWidget;
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveLife();

	UFUNCTION(BlueprintNativeEvent, Category = "GameMode")
	void MoveToGameOver();

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	int32 Life = 3;
};
