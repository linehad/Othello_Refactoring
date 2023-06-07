// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPlayerController.h"
#include "GameInfoStruct.h"
#include "ServerGameStateBase.h"
#include "GameBoard.h"
#include "ScoreUserWidget.h"
#include "OthelloPices_UserWidget.h"
#include "OthelloGameModeBase.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

void AOthelloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	// SetInputMode에 FInputModeGameAndUI를 넣어두면 게임과 UI모두 입력이 가능한 상태가 된다.
	SetInputMode(FInputModeGameAndUI());

	GameMode = GetWorld()->GetAuthGameMode<AOthelloGameModeBase>();
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

	//GameState->SetIndex(boardIndex);
	GameMode->Placement(x, y);
}

void AOthelloPlayerController::Server_TimeOut_Implementation()
{
	GameMode->TimeOut();
}
