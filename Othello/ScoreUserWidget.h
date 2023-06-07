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

	// ��Ʈ �̹����� ������ �迭
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UTexture2D*> BigFont;

		// �ð� �̹���
		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Time_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Time_Image1 = nullptr;

	// �浹 ���� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Black_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Black_Image1 = nullptr;

	// �鵹 ���� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* White_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* White_Image1 = nullptr;

	// �ִϸ��̼� ���ε�
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

	// ���ӿ��� ���� ���ε� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* GameOver_GridPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinBlack_Image = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinWhite_Image = nullptr;

	// ���� ��ư ���ε�
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Quit_Button = nullptr;

	// ���ӿ��� �ؽ�Ʈ ���ε�
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Win_Text = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Draw_Text = nullptr;

private:
	// ���� ���
	UFUNCTION()
		void Placement(FName EventName, const FBoardInfoStruct& Data);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetSeconds() { seconds = GameInfoStruct.Time; }

	// ������ ����ϴ� �Լ�
	UFUNCTION()
		void SetScore(int BScore, int WScore);
	// �ð��� ���ϴ� �Լ�
	UFUNCTION()
		void SetTime(int Time);

	UFUNCTION()
		void CountDown();
	UFUNCTION()
		void StartTimer();

	// ������ ������ �� ȣ��� �Լ���
	void GameOver(int8 gameWinner);

	UFUNCTION()
	void QuitGame();
};
