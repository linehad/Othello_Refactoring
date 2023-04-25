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
	UPROPERTY()
		int limitTime = 0;
	UPROPERTY()
		int boardSize = 0;

	int changeCount = 0;
	const int ENTRY = 0;

	UPROPERTY()
		int beforeIndex = 0;
	UPROPERTY()
		bool bcheckTurn = false;
	UPROPERTY()
		int othelloArrIndex;

	int seconds = 0;
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")

	void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget){ StartingWidgetClass = StartingWidget;	}

	// 다음 턴이 되었을 때 실행 할 함수
	UFUNCTION(Server, Reliable)
		void SetOthelloNextturn();
	void SetOthelloNextturn_Implementation();

	// 눌린 버튼을 설정할 함수
	UFUNCTION(Server, Reliable)
		void SetOthelloArrIndex(int x, int y);
	void SetOthelloArrIndex_Implementation(int x, int y);

	// 처음 시작시 보드의 사이즈와 제한 시간을 설정하는 함수
	UFUNCTION(NetMulticast, Reliable)
		void SetGameData(int size, int t);
	void SetGameData_Implementation(int size, int t);

	// 다음 턴이 되었을 때 실행 할 함수
	UFUNCTION()
		void OthelloNextturn(int othelloArrIndex);

	// 게임의 턴을 뒤집는 함수
	UFUNCTION()
		void OthelloChangeturn();
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;
};
