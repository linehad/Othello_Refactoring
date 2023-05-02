// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	bool b_SizeButton = false;
	bool b_TimeButton = false;

	int time = 0;
	int size = 0;
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void StartButtonCallback();

	UFUNCTION(BlueprintCallable)
		void EndButtonCallback();

	UFUNCTION(BlueprintCallable)
		void SizeButtonActivate();
	UFUNCTION(BlueprintCallable)
		void TimeButtonActivate();

	// ������ ���� Size ���� ����

	UPROPERTY(meta = (BindWidget))
		class UButton* Size_Button_0 = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* Size_Button_1 = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* Size_Button_2 = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* Size_Button_3 = nullptr;

	UFUNCTION(BlueprintCallable)
		void SizeButton0_Callback();

	UFUNCTION(BlueprintCallable)
		void SizeButton1_Callback();

	UFUNCTION(BlueprintCallable)
		void SizeButton2_Callback();

	UFUNCTION(BlueprintCallable)
		void SizeButton3_Callback();

	// ������ ���� �ð� ����
	UPROPERTY(meta = (BindWidget))
		class UButton* Time_Button_0 = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* Time_Button_1 = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* Time_Button_2 = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* Time_Button_3 = nullptr;

	UFUNCTION(BlueprintCallable)
		void TimeButton0_Callback();

	UFUNCTION(BlueprintCallable)
		void TimeButton1_Callback();

	UFUNCTION(BlueprintCallable)
		void TimeButton2_Callback();

	UFUNCTION(BlueprintCallable)
		void TimeButton3_Callback();


	// ����, ���� ��ư
	UPROPERTY(meta = (BindWidget))
		class UButton* Start_Button = nullptr;
	UPROPERTY(meta = (BindWidget))
		class UButton* End_Button = nullptr;
};
