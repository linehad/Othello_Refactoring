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
	// SetInputMode�� FInputModeGameAndUI�� �־�θ� ���Ӱ� UI��� �Է��� ������ ���°� �ȴ�.
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