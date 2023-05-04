// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloServer_UserWidget.h"
#include "ServerPlayerController.h"
#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include "Kismet/GameplayStatics.h"

void UOthelloServer_UserWidget::NativeConstruct()
{
	server_Button->OnClicked.AddDynamic(this, &UOthelloServer_UserWidget::ServerButtonCallback);
	client_Button->OnClicked.AddDynamic(this, &UOthelloServer_UserWidget::ClientButtonCallback);
	ip_EditableTextBox->OnTextChanged.AddDynamic(this, &UOthelloServer_UserWidget::IP_EditableTextBoxCallback);
}

void UOthelloServer_UserWidget::ServerButtonCallback()
{
	// auto UserWidget = LoadObject<UClass>(NULL, TEXT("/Game/BP_MainMenu.BP_MainMenu_C"), NULL, LOAD_None, NULL);
	// AServerPlayerController* Controller = Cast<AServerPlayerController>(GetWorld()->GetFirstPlayerController());
	// Controller->ChangeWidget(UserWidget);
	UGameplayStatics::OpenLevel(this, FName(TEXT("Othello")), true, FString(TEXT("listen")));
}

void UOthelloServer_UserWidget::ClientButtonCallback()
{
	FName ip = FName(*ip_EditableTextBox->GetText().ToString());
	UGameplayStatics::OpenLevel(this, ip, true);
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
