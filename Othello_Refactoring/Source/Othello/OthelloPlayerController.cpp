// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPlayerController.h"
#include "GameInfoStruct.h"
#include "ServerGameStateBase.h"
#include "GameBoard.h"
#include "ScoreUserWidget.h"
#include "OthelloPices_UserWidget.h"
#include "SinglePlayGameMode.h"
#include "OthelloGameModeBase.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

void AOthelloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	// SetInputMode�� FInputModeGameAndUI�� �־�θ� ���Ӱ� UI��� �Է��� ������ ���°� �ȴ�.
	SetInputMode(FInputModeGameAndUI());

	GameMode = GetWorld()->GetAuthGameMode<AOthelloGameModeBase>();
	SingleGameMod = GetWorld()->GetAuthGameMode<ASinglePlayGameMode>();
	GameState= Cast<AServerGameStateBase>(GetWorld()->GetGameState());
}

void AOthelloPlayerController::ResetBoardWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (BoardWidget != nullptr)
	{
		BoardWidget->RemoveFromViewport();
		BoardWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		BoardWidget = CreateWidget(GetWorld(), NewWidgetClass);
		CurrentWidget = BoardWidget;

		if (BoardWidget != nullptr)
		{
			BoardWidget->AddToViewport();
			Cast<UGameBoard>(BoardWidget)->StartSet();
		}
	}
}

void AOthelloPlayerController::AddScoreWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (ScoreWidget != nullptr)
	{
		ScoreWidget->RemoveFromViewport();
		ScoreWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		ScoreWidget = CreateWidget(GetWorld(), NewWidgetClass);

		if (ScoreWidget != nullptr)
		{
			ScoreWidget->AddToViewport();
			Cast<UScoreUserWidget>(ScoreWidget)->StartTimer();
		}
	}
}

void AOthelloPlayerController::ChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
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

void AOthelloPlayerController::Server_SetBoardCoordinate_Implementation(int x, int y)
{
	const int32 boardIndex = (GameState->GetGameInfoStruct().Size * y) + x;

	if (GameMode)
	{
		GameMode->Placement(x, y);
	}
	else if (SingleGameMod)
	{
		SingleGameMod->Placement(x, y);
	}
}

void AOthelloPlayerController::Server_TimeOut_Implementation()
{
	if (GameMode)
	{
		GameMode->TimeOut();
	}
	else if (SingleGameMod)
	{
		SingleGameMod->TimeOut();
	}
}
