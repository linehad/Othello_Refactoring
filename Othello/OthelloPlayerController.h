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
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidget = nullptr;

	UFUNCTION()
		int GetBeforeWhiteScore() { return beforeWhiteScore; }
	UFUNCTION()
		int GetBeforeBlackScore() { return beforeBlackScore; }
	UFUNCTION()
		int GetWhiteScore() { return whiteScore; }
	UFUNCTION()
		int GetBlackScore() { return blackScore; }

	UFUNCTION(Client, Reliable)
		void OthelloNextturn(int index);
	void OthelloNextturn_Implementation(int index);

	UFUNCTION(Client, Reliable)
		void PutPieces(int index);
	void PutPieces_Implementation(int index);
	/*위젯을 업데이트 하는 RPC 함수*/

	UFUNCTION(Client, Reliable)
		void ResetBoardWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time);
	void ResetBoardWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time);

	UFUNCTION(Client, Reliable)
		void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void ChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);

public:
	// 눌린 버튼을 설정할 함수
	UFUNCTION(Server, Reliable)
		void SetBoardCoordinate(int x, int y);
	void SetBoardCoordinate_Implementation(int x, int y);

	UFUNCTION(Server, Reliable)
		void Server_ReverseTurn();
	void Server_ReverseTurn_Implementation();

	UPROPERTY()
		UUserWidget* CurrentWidget = nullptr;
	UPROPERTY()
		UUserWidget* BoardWidget = nullptr;
};