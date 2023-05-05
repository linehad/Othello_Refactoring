// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameBoard.generated.h"

/**
 *
 */
UCLASS()
class OTHELLO_API UGameBoard : public UUserWidget
{
	GENERATED_BODY()
private:
	int boardSize = 0;
	int limitTime = 0;

	int seconds = 0;

	int beforWhiteScore = 0;
	int beforBlackScore = 0;

	const int DRAW = 0;
	const int BLACK = 1;
	const int WHITE = 2;
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
		TArray <class UOthelloPices_UserWidget*>arrOthelloButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf <class UOthelloPices_UserWidget>OthelloButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UUniformGridPanel* Board_UniformGridPanel = nullptr;

	// 흑돌, 백돌 이미지
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* WhiteImage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* BlackImage = nullptr;

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

	// 게임오버 위젯 바인딩 이미지
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* GameOver_GridPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinBlack_Image = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinWhite_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Quit_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UTexture2D*> BigFont;

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


	// 게임의 흐름을 담당하는 함수들
public:
	// 처음 시작할 때 호출할 함수
	void StartSet(int32 size, int32 time);

	// 1차원 배열을 2차월 배열 처럼 쓰기 위한 함수
	UFUNCTION()
		inline int16 Board_index(int x, int y) { return (boardSize * y) + x; }

	// 점수를 계산하는 함수
	UFUNCTION()
		void SetScore(int BScore, int WScore);
	UFUNCTION()
		void SetSeconds() { seconds = limitTime; }

	// 시간을 정하는 함수
	UFUNCTION()
		void SetTime(int Time);

	UFUNCTION()
		void CountDown();
	UFUNCTION()
		void StartTimer();

	// 게임이 끝났을 때 호출될 함수들
	UFUNCTION()
		void GameOver(int8 gameWinner); // 0 무승부 1 흑돌 승 2 백돌 승
	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void NextTurn();

	UFUNCTION()
		void SetwhiteScore(int score) { beforWhiteScore = score; }
	UFUNCTION()
		void SetblackScore(int score) { beforBlackScore = score; }

	inline void SetBoardSize(int size) { boardSize = size; }
	inline int GetBoardSize() { return boardSize; }
};