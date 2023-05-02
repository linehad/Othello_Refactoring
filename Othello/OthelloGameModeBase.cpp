// Copyright Epic Games, Inc. All Rights Reserved.


#include "OthelloGameModeBase.h"
#include "OthelloPlayerController.h"
#include <Blueprint/UserWidget.h>

void AOthelloGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AOthelloGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(NewPlayer);

		if (Controller)
		{
			Controller->ChangeWidget(StartingWidgetClass);
		}
}

void AOthelloGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
