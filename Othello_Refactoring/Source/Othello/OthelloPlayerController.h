// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OthelloPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API AOthelloPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY()
		int whiteScore = 0;
	UPROPERTY()
		int blackScore = 0;
	int beforeBlackScore = 0;
	int beforeWhiteScore = 0;

	class AOthelloGameModeBase* GameMode;
	class ASinglePlayGameMode* SingleGameMod;
	class AServerGameStateBase* GameState;

	UPROPERTY()
		UUserWidget* CurrentWidget = nullptr;
	UPROPERTY()
		UUserWidget* BoardWidget = nullptr;
	UPROPERTY()
		UUserWidget* ScoreWidget = nullptr;
public:
	virtual void BeginPlay() override;

	UFUNCTION()
		int GetBeforeWhiteScore() { return beforeWhiteScore; }
	UFUNCTION()
		int GetBeforeBlackScore() { return beforeBlackScore; }
	UFUNCTION()
		int GetWhiteScore() { return whiteScore; }
	UFUNCTION()
		int GetBlackScore() { return blackScore; }

	/*������ ������Ʈ �ϴ� RPC �Լ�*/

	UFUNCTION(Client, Reliable)
		void ResetBoardWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time);
	void ResetBoardWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time);

	UFUNCTION(Client, Reliable)
		void AddScoreWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void AddScoreWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(Client, Reliable)
		void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void ChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);

public:
	// ���� ��ư�� ������ �Լ�
	UFUNCTION(Server, Reliable)
		void Server_SetBoardCoordinate(int x, int y);
	void Server_SetBoardCoordinate_Implementation(int x, int y);

	// �ð��� �ʰ��Ǹ� ����Ǵ� �Լ�
	UFUNCTION(Server, Reliable)
		void Server_TimeOut();
	void Server_TimeOut_Implementation();
};