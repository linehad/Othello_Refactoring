// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameInfoStruct.h"
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

	FGameInfoStruct GameInfoStruct;
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

	/*위젯을 업데이트 하는 RPC 함수*/

	UFUNCTION(Client, Reliable)
		void ResetBoardWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time);
	void ResetBoardWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass, int32 size, int32 time);

	UFUNCTION(Client, Reliable)
		void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void ChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION()
		void GameInfoStructUpdated(FName PropertyName, const FGameInfoStruct& Data);
public:
	// 눌린 버튼을 설정할 함수
	UFUNCTION(Server, Reliable)
		void SetOthelloArrIndex(int x, int y);
	void SetOthelloArrIndex_Implementation(int x, int y);

	UPROPERTY()
		UUserWidget* CurrentWidget = nullptr;
	UPROPERTY()
		UUserWidget* BoardWidget = nullptr;
};