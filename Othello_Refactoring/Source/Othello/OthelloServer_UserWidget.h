// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OthelloServer_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UOthelloServer_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

		UPROPERTY(meta = (BindWidget))
		class UButton* server_Button = nullptr;
		UPROPERTY(meta = (BindWidget))
			class UButton* client_Button = nullptr;
		UPROPERTY(meta = (BindWidget))
			class UButton* SinglePlay_Button = nullptr;

		UPROPERTY(meta = (BindWidget))
			class UEditableTextBox* ip_EditableTextBox = nullptr;

		UFUNCTION(BlueprintCallable)
			void ServerButton_Callback();
		UFUNCTION(BlueprintCallable)
			void ClientButton_Callback();
		UFUNCTION(BlueprintCallable)
			void SinglePlayButton_Callback();

		UFUNCTION(BlueprintCallable)
			void IP_EditableTextBoxCallback(const FText& Text);
};
