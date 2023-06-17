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

	int16 othelloArrIndex; // ���� ��ġ�� ������ ����

	UPROPERTY()
		TArray <int32> othelloBoard; // ���� ������ ���� ��Ȳ

	TArray<int32>aiBoard;

	FTimerHandle AiTimer; // ai �Լ��� �ʹ� ������ ����Ǿ� ������ ������ �ʴ´�. ���� �Ϻη� �ʰ� �����ϱ� ���� �ڵ鷯 ����
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget) { StartingWidgetClass = StartingWidget; }

	// ó�� ���۽� ������ ������� ���� �ð��� �����ϴ� �Լ�
	void SetGameData(int size, int time);

	// ������ ���� ������ �Լ�
	void ReverseTurn(); // �ð� �ʰ� �Ѿ�� ���� ������ ���� �Ѿ� ���� �� ��� ȣ��

	UFUNCTION()
		void Placement(int32 pX, int32 py); // ���� �Ǿ��� �� ����Ǵ� �Լ�
	UFUNCTION()
		void AiPlacement(); // ���� �Ǿ��� �� ����Ǵ� �Լ�
		void TimeOut(); // ���� �Ǿ��� �� ����Ǵ� �Լ�

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }

	// �̺�Ʈ ��� ������
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
	void SetAi(); // ���� ���۽� ���带 �ʱ�ȭ�ϰ� ����ġ�� �����Ѵ�. 
};
