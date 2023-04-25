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
	int8 piece = 0; // 놓아져 있는 돌이 흑돌인지 백돌인지 체크
	bool bturn = true; // 현재 놓을 차례가 흑돌인지 백돌인지 체크
	int32 xPos = 0;
	int32 yPos = 0;

	bool possiblePiece = false; // 둘수 있는지 없는지 판별
public:
	bool bhit = false;

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
	void SetData(int y, int x);
	UFUNCTION()
		void OnBtnClick();
	UFUNCTION()
		void Changeturn();

		void ReversePice();
	UFUNCTION()
		int32 GetX() { return xPos; }
	UFUNCTION()
		int32 GetY() { return yPos; }
	UFUNCTION()
		int32 GetPice() { return piece; }
	UFUNCTION()
		bool GetTurn() { return bturn; }
	UFUNCTION()
		bool GetPossiblePice() { return possiblePiece; }
	UFUNCTION()
		void PossiblePice(); // 클릭 가능
	UFUNCTION()
		void UnPossiblePice(); // 클릭 불가능
};
