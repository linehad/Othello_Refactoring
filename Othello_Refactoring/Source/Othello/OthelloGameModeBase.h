#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameInfoStruct.h"
#include "OthelloGameModeBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameInfoStructUpdated, FName, PropertyName, const FGameInfoStruct&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBoardUpdatedEvent, FName, PropertyName, const FBoardInfoStruct&, BoardInfoStruct);

UCLASS()
class OTHELLO_API AOthelloGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FGameInfoStruct GameInfoStruct;
	UPROPERTY()
		FBoardInfoStruct BoardInfoStruct;

		int16 othelloArrIndex; // 눌린 위치를 저장할 변수

	UPROPERTY()
	TArray <int32> othelloBoard; // 보드 게임의 현재 상황
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget){ StartingWidgetClass = StartingWidget;	}

	// 처음 시작시 보드의 사이즈와 제한 시간을 설정하는 함수
		void SetGameData(int size, int time);

	// 게임의 턴을 뒤집는 함수
		void ReverseTurn(); // 시간 초가 넘어가는 등의 이유로 턴이 넘어 가야 할 경우 호출

	UFUNCTION()
		void Placement(int32 pX, int32 py); // 착수 되었을 때 실행되는 함수

	UFUNCTION()
		void TimeOut(); // 착수 되었을 때 실행되는 함수

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }

	// 이벤트 멤버 변수들
	UPROPERTY()
	FGameInfoStructUpdated OnGameInfoUpdated;

	UPROPERTY()
		FBoardUpdatedEvent OnTurnUpdated;
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BoardWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> ScoreWidgetClass;
	UPROPERTY()
		UUserWidget* CurrentWidget;

private:
	void RecordPiece(int32 index, TArray<int32> ReversePos);
};