// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameInfoStruct.h"
#include "SinglePlayGameMode.generated.h"

enum AIPosition
{
	BEST_POSITION = 15,
	VERY_GOOD_POSITION = 14,
	GOOD_POSITION = 13,
	NORMAL_POSITION = 12,
	NOT_GOOD_POSITION = 11,
	BAD_POSITION = 10,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSingleGameInfoStructUpdated, FName, PropertyName, const FGameInfoStruct&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSingleBoardUpdatedEvent, FName, PropertyName, const FBoardInfoStruct&, BoardInfoStruct);

UCLASS()
class OTHELLO_API ASinglePlayGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		FGameInfoStruct GameInfoStruct;
	UPROPERTY()
		FBoardInfoStruct BoardInfoStruct;

	UPROPERTY()
		UUserWidget* CurrentWidget = nullptr;
	UPROPERTY()
		UUserWidget* BoardWidget = nullptr;
	UPROPERTY()
		UUserWidget* ScoreWidget = nullptr;

	int16 othelloArrIndex; // 눌린 위치를 저장할 변수

	UPROPERTY()
		TArray <int32> othelloBoard; // 보드 게임의 현재 상황

	TArray<int32>aiBoard;

	FTimerHandle AiTimer; // ai 함수가 너무 빠르게 실행되어 동작이 보이지 않는다. 따라서 일부러 늦게 실행하기 위한 핸들러 선언
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget) { StartingWidgetClass = StartingWidget; }

	// 처음 시작시 보드의 사이즈와 제한 시간을 설정하는 함수
	void SetGameData(int size, int time);

	// 게임의 턴을 뒤집는 함수
	void ReverseTurn(); // 시간 초가 넘어가는 등의 이유로 턴이 넘어 가야 할 경우 호출

	UFUNCTION()
		void Placement(int32 pX, int32 py); // 착수 되었을 때 실행되는 함수
	UFUNCTION()
		void AiPlacement(); // 착수 되었을 때 실행되는 함수
		void TimeOut(); // 착수 되었을 때 실행되는 함수

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }

	// 이벤트 멤버 변수들
	UPROPERTY()
		FSingleGameInfoStructUpdated OnGameInfoUpdated;

	UPROPERTY()
		FSingleBoardUpdatedEvent OnTurnUpdated;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BoardWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> ScoreWidgetClass;

private:
	void RecordPiece(int32 index, TArray<int32> ReversePos);
	void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void SetAi(); // 게임 시작시 보드를 초기화하고 가중치를 설정한다. 
};
