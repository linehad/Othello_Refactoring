// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ServerGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API AServerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		int limitTime = 0;
	UPROPERTY()
		int boardSize = 0;

public:

	UFUNCTION()
		int GetBoardSize() { return boardSize; }

	UFUNCTION()
		int GetLimitTime() { return limitTime; }
};
