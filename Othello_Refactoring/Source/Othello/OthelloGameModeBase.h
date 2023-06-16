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

		int16 othelloArrIndex; // ���� ��ġ�� ������ ����

	UPROPERTY()
	TArray <int32> othelloBoard; // ���� ������ ���� ��Ȳ
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget){ StartingWidgetClass = StartingWidget;	}

	// ó�� ���۽� ������ ������� ���� �ð��� �����ϴ� �Լ�
		void SetGameData(int size, int time);

	// ������ ���� ������ �Լ�
		void ReverseTurn(); // �ð� �ʰ� �Ѿ�� ���� ������ ���� �Ѿ� ���� �� ��� ȣ��

	UFUNCTION()
		void Placement(int32 pX, int32 py); // ���� �Ǿ��� �� ����Ǵ� �Լ�

	UFUNCTION()
		void TimeOut(); // ���� �Ǿ��� �� ����Ǵ� �Լ�

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }

	// �̺�Ʈ ��� ������
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