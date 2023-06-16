// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloServer_UserWidget.h"
#include "ServerPlayerController.h"
#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include "Kismet/GameplayStatics.h"

void UOthelloServer_UserWidget::NativeConstruct()
{
	server_Button->OnClicked.AddDynamic(this, &UOthelloServer_UserWidget::ServerButton_Callback);
	client_Button->OnClicked.AddDynamic(this, &UOthelloServer_UserWidget::ClientButton_Callback);
	SinglePlay_Button->OnClicked.AddDynamic(this, &UOthelloServer_UserWidget::SinglePlayButton_Callback);

	ip_EditableTextBox->OnTextChanged.AddDynamic(this, &UOthelloServer_UserWidget::IP_EditableTextBoxCallback);
}

void UOthelloServer_UserWidget::ServerButton_Callback()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("Othello")), true, FString(TEXT("listen")));
}

void UOthelloServer_UserWidget::ClientButton_Callback()
{
	FName ip = FName(*ip_EditableTextBox->GetText().ToString());
	UGameplayStatics::OpenLevel(this, ip, true);
}

void UOthelloServer_UserWidget::SinglePlayButton_Callback()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("OthelloAI")));
}

void UOthelloServer_UserWidget::IP_EditableTextBoxCallback(const FText& Text)
{
	if (ip_EditableTextBox->GetText().ToString().Len() <= 0)
	{
		client_Button->SetIsEnabled(false);
	}
	else {
		client_Button->SetIsEnabled(true);
	}
}
