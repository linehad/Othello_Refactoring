// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OthelloGameModeBase.generated.h"

/**
 * 
 */ 
UCLASS()
class OTHELLO_API AOthelloGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
		int16 limitTime = 0;
		int16 boardSize = 0;

	int16 changeCount = 0;
	const int16 ENTRY = 0;

		int16 beforeIndex = 0;
		bool bcheckTurn = false;
		int16 othelloArrIndex; // 눌린 위치를 저장할 변수

	TArray <int8> arrOthelloButton; // 보드 게임의 현재 상황
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget){ StartingWidgetClass = StartingWidget;	}

	// 다음 턴이 되었을 때 실행 할 함수
	UFUNCTION(Server, Reliable)
		void SetOthelloNextturn();
	void SetOthelloNextturn_Implementation();

	// 처음 시작시 보드의 사이즈와 제한 시간을 설정하는 함수
		void SetGameData(int size, int time);

	// 다음 턴이 되었을 때 실행 할 함수
	UFUNCTION()
		void OthelloNextTurn(int othelloArrIndex);

	// 게임의 턴을 뒤집는 함수
	UFUNCTION()
		void OthelloChangeTurn();

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BoardWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;
};
