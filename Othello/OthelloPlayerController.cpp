// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPlayerController.h"
#include "ServerGameStateBase.h"
#include "GameBoard.h"
#include "OthelloPices_UserWidget.h"
#include "OthelloGameModeBase.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>
#include "Kismet/GameplayStatics.h"

void AOthelloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	// SetInputMode에 FInputModeGameAndUI를 넣어두면 게임과 UI모두 입력이 가능한 상태가 된다.
	SetInputMode(FInputModeGameAndUI());
}

void AOthelloPlayerController::OthelloNextturn_Implementation(int ButtonIndex)
{
	AServerGameStateBase* OthelloState = Cast<AServerGameStateBase>(GetWorld()->GetGameState());
	AOthelloGameModeBase* GameMode = Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UGameBoard* OthelloBoard = Cast<UGameBoard>(CurrentWidget);
	OthelloBoard->SetSeconds();
	OthelloBoard->SetTime(OthelloState->GetLimitTime());

	GameMode->OthelloNextturn(ButtonIndex);
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