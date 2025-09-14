// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSpace.h"

#include "Asteroid.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void AGameModeSpace::BeginPlay()
{
	Super::BeginPlay();

	Score = 0;
	
	if (ScoreWidgetClass)
	{
		ScoreWidget = CreateWidget<UUserWidget>(GetWorld(), ScoreWidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();

			if (UTextBlock* TextBlock = Cast<UTextBlock>(ScoreWidget->GetWidgetFromName(TEXT("ScoreVar"))))
			{
				TextBlock->SetText(FText::AsNumber(Score));
			}
		}
	}
}

void AGameModeSpace::AddScore(int32 Amount)
{
	Score += Amount;
	
	if (ScoreWidget)
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(ScoreWidget->GetWidgetFromName(TEXT("ScoreVar"))))
		{
			TextBlock->SetText(FText::AsNumber(Score));
		}
	}
}

void AGameModeSpace::RemoveLife()
{
	switch (Life)
	{
		case 3:
			{
				if (UImage* Heart = Cast<UImage>(ScoreWidget->GetWidgetFromName(TEXT("Heart"))))
				{
					Heart->SetVisibility(ESlateVisibility::Hidden);
					Life -= 1;
				}

				break;
			}
		case 2:
			{
				if (UImage* Heart = Cast<UImage>(ScoreWidget->GetWidgetFromName(TEXT("Heart_1"))))
				{
					Heart->SetVisibility(ESlateVisibility::Hidden);
					Life -= 1;
				}

				break;
			}
		case 1:
			{
				if (UImage* Heart = Cast<UImage>(ScoreWidget->GetWidgetFromName(TEXT("Heart_2"))))
				{
					Heart->SetVisibility(ESlateVisibility::Hidden);
					Life -= 1;
				}

				MoveToGameOver();
				
				break;
			}
		default:
			{
				break;
			}
	}
}

void AGameModeSpace::MoveToGameOver_Implementation()
{
	//En BP
}
