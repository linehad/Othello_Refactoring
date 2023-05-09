#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OthelloGameModeBase.generated.h"

USTRUCT()
struct FGameInfoStruct
{
	GENERATED_BODY()

public:
		int32 Size;
		int32 Time;
		int8 GameTurn = 0;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameInfoStructUpdated, FName, PropertyName, const FGameInfoStruct&, Data);

UCLASS()
class OTHELLO_API AOthelloGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	FGameInfoStruct GameInfoStruct;
	//UPROPERTY(BlueprintReadWrite, Category = "GameInfo")
	int16 changeCount = 0;
	const int16 ENTRY = 0;

		int16 beforeIndex = 0;
		bool bcheckTurn = false;
		int16 othelloArrIndex; // 눌린 위치를 저장할 변수

	UPROPERTY()
	TArray <int32> othelloBoard; // 보드 게임의 현재 상황
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
		void OthelloNextTurn(int arrIndex);

	// 게임의 턴을 뒤집는 함수
		void OthelloChangeTurn(); // 시간 초가 넘어가는 등의 이유로 턴이 넘어 가야 할 경우 호출

	UFUNCTION()
		TArray<int8> OthelloChangeTurn(int32 pX, int32 py); // 게임턴에 따라 현재 플레이어 색의 돌의 착수 가능 위치를 표시해주는 함수

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }

	FGameInfoStructUpdated OnGameInfoUpdated;
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