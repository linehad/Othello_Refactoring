// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPlayerController.h"
#include "ServerGameStateBase.h"
#include "GameBoard.h"
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

	AOthelloGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AOthelloGameModeBase>();
	if (GameMode && HasAuthority()) //클라에서는 접근이 불가능하기 때문에 서버에서만 실행이 된다.
	{
		GameMode->OnGameInfoUpdated.AddDynamic(this, &AOthelloPlayerController::Server_GameInfoStructUpdated);
	}
}

void AOthelloPlayerController::OthelloNextturn_Implementation(int ButtonIndex)
{
	AServerGameStateBase* OthelloState = Cast<AServerGameStateBase>(GetWorld()->GetGameState());
	AOthelloGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AOthelloGameModeBase>();
	UGameBoard* OthelloBoard = Cast<UGameBoard>(CurrentWidget);
	OthelloBoard->SetSeconds();
	OthelloBoard->SetTime(OthelloState->GetLimitTime());

	GameMode->OthelloNextTurn(ButtonIndex);
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
			Cast<UGameBoard>(BoardWidget)->StartSet(size, time);
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

	if (CurrentWidget != nullptr && CurrentWidget == Cast<UGameBoard>(CurrentWidget))
	{
		Cast<UGameBoard>(CurrentWidget)->StartTimer();
	}
}

void AOthelloPlayerController::SetOthelloArrIndex_Implementation(int x, int y)
{
	//othelloArrIndex = (Cast<AServerGameStateBase>(GetWorld()->GetGameState())->GetBoardSize() * y) + x;
}

void AOthelloPlayerController::Server_GameInfoStructUpdated(FName PropertyName, const FGameInfoStruct& Data)
{
	if (HasAuthority())
	{
		if (PropertyName == TEXT("GameInfo"))
		{
			UE_LOG(LogTemp, Log, TEXT("server "));
			Multicast_UpdateGameInfo(Data);
		}

	}
}

void AOthelloPlayerController::Multicast_UpdateGameInfo_Implementation(const FGameInfoStruct& Data)
{
	GameInfoStruct = Data;
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("server "));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("client "));
	}
}
