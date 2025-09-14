// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlayerController.h"
#include "EnhancedInputSubsystems.h"

void ASpacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Montrer le curseur en tout temps
	bShowMouseCursor = true;
	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_SpaceShooter)
			{
				Subsystem->AddMappingContext(IMC_SpaceShooter, 0);
			}
		}
	}
}