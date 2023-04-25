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
		void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void ChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass);
	UPROPERTY()
		UUserWidget* CurrentWidget = nullptr;
};