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

	// ���� ���� �Ǿ��� �� ���� �� �Լ�
	UFUNCTION(Server, Reliable)
		void SetOthelloNextturn();
	void SetOthelloNextturn_Implementation();

	// ���� ��ư�� ������ �Լ�
	UFUNCTION(Server, Reliable)
		void SetOthelloArrIndex(int x, int y);
	void SetOthelloArrIndex_Implementation(int x, int y);

	// ó�� ���۽� ������ ������� ���� �ð��� �����ϴ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
		void SetGameData(int size, int t);
	void SetGameData_Implementation(int size, int t);

	// ���� ���� �Ǿ��� �� ���� �� �Լ�
	UFUNCTION()
		void OthelloNextturn(int othelloArrIndex);

	// ������ ���� ������ �Լ�
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
