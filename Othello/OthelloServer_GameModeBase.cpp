// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloServer_GameModeBase.h"
#include "ServerPlayerController.h"
#include <Blueprint/UserWidget.h>

void AOthelloServer_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOthelloServer_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AServerPlayerController* Controller = Cast<AServerPlayerController>(NewPlayer);

	if (Controller)
	{
		Controller->ChangeWidget(StartingWidgetClass);
	}
}