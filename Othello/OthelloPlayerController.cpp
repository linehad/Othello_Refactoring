// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPlayerController.h"
#include <Blueprint/UserWidget.h>

void AOthelloPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	// SetInputMode에 FInputModeGameAndUI를 넣어두면 게임과 UI모두 입력이 가능한 상태가 된다.
	SetInputMode(FInputModeGameAndUI());
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

//void AOthelloPlayerController::ShowWidget_Implementation() {
//	UE_LOG(LogTemp, Warning, TEXT("PlayerController ShowWidget Called"));
//}

