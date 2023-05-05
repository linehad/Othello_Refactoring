// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OthelloServer_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API AOthelloServer_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		int time = 0;
	UPROPERTY()
		int boardSize;

public:
	UFUNCTION()
		void SetTime(int t) { time = t; }
	UFUNCTION()
		void SetBoardSize(int size) { boardSize = size; }
	UFUNCTION()
		int GetTime() { return time; }
	UFUNCTION()
		int GetBoardSize() { return boardSize; }

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;

};
