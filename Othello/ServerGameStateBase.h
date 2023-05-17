// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ServerGameStateBase.generated.h"

UCLASS()
class OTHELLO_API AServerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
		int limitTime = 0;
		int boardSize = 0;

	TArray <int8> arrOthelloButton;
public:
	UFUNCTION()
		void SetData(int32 time, int32 size);
	
	UFUNCTION()
		int GetBoardSize() { return boardSize; }

	UFUNCTION()
		int GetLimitTime() { return limitTime; }
};
