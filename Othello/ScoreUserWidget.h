// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInfoStruct.h"
#include "ScoreUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UScoreUserWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	FTimerHandle timerHandle;

	const int8 DRAW = 0;
	const int8 BLACK_WIN = 1;
	const int8 WHITE_WIN = 2;

	int32 seconds = 0;
	int32 timeLimit = 0;

	int32 lastWhiteScore = 0;
	int32 lastBlackScore = 0;

	FGameInfoStruct GameInfoStruct;

	class AServerGameStateBase* GameState;
protected:

	// 폰트 이미지를 저장할 배열
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UTexture2D*> BigFont;

		// 시간 이미지
		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Time_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Time_Image1 = nullptr;

	// 흑돌 점수 이미지
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Black_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Black_Image1 = nullptr;

	// 백돌 점수 이미지
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* White_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* White_Image1 = nullptr;

	// 애니메이션 바인딩
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Time_Image0_Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Time_Image1_Anim = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Black_Image0_Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Black_Image1_Anim = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* White_Image0_Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* White_Image1_Anim = nullptr;

	// 게임오버 위젯 바인딩 이미지
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* GameOver_GridPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinBlack_Image = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinWhite_Image = nullptr;

	// 종료 버튼 바인딩
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Quit_Button = nullptr;

	// 게임오버 텍스트 바인딩
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Win_Text = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Draw_Text = nullptr;

private:
	// 착수 기능
	UFUNCTION()
		void Placement(FName EventName, const FBoardInfoStruct& Data);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetSeconds() { seconds = GameInfoStruct.Time; }

	// 점수를 계산하는 함수
	UFUNCTION()
		void SetScore(int BScore, int WScore);
	// 시간을 정하는 함수
	UFUNCTION()
		void SetTime(int Time);

	UFUNCTION()
		void CountDown();
	UFUNCTION()
		void StartTimer();

	// 게임이 끝났을 때 호출될 함수들
	void GameOver(int8 gameWinner);

	UFUNCTION()
	void QuitGame();
};
