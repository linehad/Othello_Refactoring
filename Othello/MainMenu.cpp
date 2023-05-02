// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "OthelloGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <Components/Button.h>

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Start_Button->SetIsEnabled(false);
	Start_Button->OnClicked.AddDynamic(this, &UMainMenu::StartButtonCallback);
	End_Button->OnClicked.AddDynamic(this, &UMainMenu::EndButtonCallback);

	Size_Button_0->OnClicked.AddDynamic(this, &UMainMenu::SizeButton0_Callback);
	Size_Button_1->OnClicked.AddDynamic(this, &UMainMenu::SizeButton1_Callback);
	Size_Button_2->OnClicked.AddDynamic(this, &UMainMenu::SizeButton2_Callback);
	Size_Button_3->OnClicked.AddDynamic(this, &UMainMenu::SizeButton3_Callback);
	
	Time_Button_0->OnClicked.AddDynamic(this, &UMainMenu::TimeButton0_Callback);
	Time_Button_1->OnClicked.AddDynamic(this, &UMainMenu::TimeButton1_Callback);
	Time_Button_2->OnClicked.AddDynamic(this, &UMainMenu::TimeButton2_Callback);
	Time_Button_3->OnClicked.AddDynamic(this, &UMainMenu::TimeButton3_Callback);
}

void UMainMenu::StartButtonCallback()
{
	Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetBoardSize(size);
	Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetTime(time);
	auto UserWidget = LoadObject<UClass>(NULL, TEXT("/Game/BP_Board.BP_Board_C"), NULL, LOAD_None, NULL);
	Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ChangeMenuWidget(UserWidget);
}

void UMainMenu::EndButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainMenu::SizeButtonActivate()
{
	if (b_SizeButton && b_TimeButton)
	{
		Start_Button->SetIsEnabled(true);
	}
}

void UMainMenu::TimeButtonActivate()
{
	if (b_SizeButton && b_TimeButton)
	{
		Start_Button->SetIsEnabled(true);
	}
}

void UMainMenu::SizeButton0_Callback()
{
	size = 6;
	Size_Button_0->SetIsEnabled(false); Size_Button_1->SetIsEnabled(true); Size_Button_2->SetIsEnabled(true); Size_Button_3->SetIsEnabled(true);
	b_SizeButton = true;
	SizeButtonActivate();
}
void UMainMenu::SizeButton1_Callback()
{
	size = 8;
	Size_Button_0->SetIsEnabled(true); Size_Button_1->SetIsEnabled(false); Size_Button_2->SetIsEnabled(true); Size_Button_3->SetIsEnabled(true);
	b_SizeButton = true;
	SizeButtonActivate();
}
void UMainMenu::SizeButton2_Callback()
{
	size = 10;
	Size_Button_0->SetIsEnabled(true); Size_Button_1->SetIsEnabled(true); Size_Button_2->SetIsEnabled(false); Size_Button_3->SetIsEnabled(true);
	b_SizeButton = true;
	SizeButtonActivate();
}
void UMainMenu::SizeButton3_Callback()
{
	size = 12;
	Size_Button_0->SetIsEnabled(true); Size_Button_1->SetIsEnabled(true); Size_Button_2->SetIsEnabled(true); Size_Button_3->SetIsEnabled(false);
	b_SizeButton = true;
	SizeButtonActivate();
}


void UMainMenu::TimeButton0_Callback()
{
	time = 30;
	Time_Button_0->SetIsEnabled(false); Time_Button_1->SetIsEnabled(true); Time_Button_2->SetIsEnabled(true); Time_Button_3->SetIsEnabled(true);
	b_TimeButton = true;
	TimeButtonActivate();
}
void UMainMenu::TimeButton1_Callback()
{
	time = 45;
	Time_Button_0->SetIsEnabled(true); Time_Button_1->SetIsEnabled(false); Time_Button_2->SetIsEnabled(true); Time_Button_3->SetIsEnabled(true);
	b_TimeButton = true;
	TimeButtonActivate();
}
void UMainMenu::TimeButton2_Callback()
{
	time = 60;
	Time_Button_0->SetIsEnabled(true); Time_Button_1->SetIsEnabled(true); Time_Button_2->SetIsEnabled(false); Time_Button_3->SetIsEnabled(true);
	b_TimeButton = true;
	TimeButtonActivate();
}
void UMainMenu::TimeButton3_Callback()
{
	time = 90;
	Time_Button_0->SetIsEnabled(true); Time_Button_1->SetIsEnabled(true); Time_Button_2->SetIsEnabled(true); Time_Button_3->SetIsEnabled(false);
	b_TimeButton = true;
	TimeButtonActivate();
}