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
		int8 gameTurn = 0;

	int16 changeCount = 0;
	const int16 ENTRY = 0;

		int16 beforeIndex = 0;
		bool bcheckTurn = false;
		int16 othelloArrIndex; // ���� ��ġ�� ������ ����

	TArray <int32> othelloBoard; // ���� ������ ���� ��Ȳ
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget){ StartingWidgetClass = StartingWidget;	}

	// ���� ���� �Ǿ��� �� ���� �� �Լ�
	UFUNCTION(Server, Reliable)
		void SetOthelloNextturn();
	void SetOthelloNextturn_Implementation();

	// ó�� ���۽� ������ ������� ���� �ð��� �����ϴ� �Լ�
		void SetGameData(int size, int time);

	// ���� ���� �Ǿ��� �� ���� �� �Լ�
		void OthelloNextTurn(int othelloArrIndex);

	// ������ ���� ������ �Լ�
		TArray<int8> OthelloChangeTurn(int32 pX, int32 py); // �����Ͽ� ���� ���� �÷��̾� ���� ���� ���� ���� ��ġ�� ǥ�����ִ� �Լ�

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
