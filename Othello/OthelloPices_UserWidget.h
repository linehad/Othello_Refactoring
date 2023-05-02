// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OthelloPices_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UOthelloPices_UserWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	// false = 흑돌, true = 백돌
	// 0 = 빈곳, 1 = 흑돌, 2 = 백돌

	int8 m_Pice = 0; // 놓아져 있는 돌이 흑돌인지 백돌인지 체크
	bool b_turn = true; // 현재 놓을 차례가 흑돌인지 백돌인지 체크
	int32 m_XPos = 0;
	int32 m_YPos = 0;

	bool m_PossiblePice = false; // 둘수 있는지 없는지 판별
public:
	bool b_hit = false;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* OthlloPices_Button = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* OthlloPices_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* WhiteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* BlackImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* Preview_WhiteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* Preview_BlackImage;

	UFUNCTION()
	void SetTurn(bool turn);
	UFUNCTION()
	void SetData(int y, int x);
	UFUNCTION()
		void OnBtnClick();
	UFUNCTION()
		void Changeturn();
	UFUNCTION()
		void ReversePice();
	UFUNCTION()
		int32 GetX() { return m_XPos; }
	UFUNCTION()
		int32 GetY() { return m_YPos; }
	UFUNCTION()
		int32 GetPice() { return m_Pice; }
	UFUNCTION()
		bool GetTurn() { return b_turn; }
	UFUNCTION()
		bool GetPossiblePice() { return m_PossiblePice; }
	UFUNCTION()
		void PossiblePice();
	UFUNCTION()
		void UnPossiblePice();
};
