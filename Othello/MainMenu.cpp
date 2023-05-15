#include "MainMenu.h"
#include "OthelloPlayerController.h"
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
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetGameData(size, time);
	}
}

void UMainMenu::EndButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainMenu::SizeButtonActivate()
{
	if (bSizeButton && bTimeButton)
	{
		Start_Button->SetIsEnabled(true);
	}
}

void UMainMenu::TimeButtonActivate()
{
	if (bSizeButton && bTimeButton)
	{
		Start_Button->SetIsEnabled(true);
	}
}

void UMainMenu::SizeButton0_Callback()
{
	size = 6;
	Size_Button_0->SetIsEnabled(false); Size_Button_1->SetIsEnabled(true); Size_Button_2->SetIsEnabled(true); Size_Button_3->SetIsEnabled(true);
	bSizeButton = true;
	SizeButtonActivate();
}

void UMainMenu::SizeButton1_Callback()
{
	size = 8;
	Size_Button_0->SetIsEnabled(true); Size_Button_1->SetIsEnabled(false); Size_Button_2->SetIsEnabled(true); Size_Button_3->SetIsEnabled(true);
	bSizeButton = true;
	SizeButtonActivate();
}
void UMainMenu::SizeButton2_Callback()
{
	size = 10;
	Size_Button_0->SetIsEnabled(true); Size_Button_1->SetIsEnabled(true); Size_Button_2->SetIsEnabled(false); Size_Button_3->SetIsEnabled(true);
	bSizeButton = true;
	SizeButtonActivate();
}
void UMainMenu::SizeButton3_Callback()
{
	size = 12;
	Size_Button_0->SetIsEnabled(true); Size_Button_1->SetIsEnabled(true); Size_Button_2->SetIsEnabled(true); Size_Button_3->SetIsEnabled(false);
	bSizeButton = true;
	SizeButtonActivate();
}


void UMainMenu::TimeButton0_Callback()
{
	time = 30;
	Time_Button_0->SetIsEnabled(false); Time_Button_1->SetIsEnabled(true); Time_Button_2->SetIsEnabled(true); Time_Button_3->SetIsEnabled(true);
	bTimeButton = true;
	TimeButtonActivate();
}
void UMainMenu::TimeButton1_Callback()
{
	time = 45;
	Time_Button_0->SetIsEnabled(true); Time_Button_1->SetIsEnabled(false); Time_Button_2->SetIsEnabled(true); Time_Button_3->SetIsEnabled(true);
	bTimeButton = true;
	TimeButtonActivate();
}
void UMainMenu::TimeButton2_Callback()
{
	time = 60;
	Time_Button_0->SetIsEnabled(true); Time_Button_1->SetIsEnabled(true); Time_Button_2->SetIsEnabled(false); Time_Button_3->SetIsEnabled(true);
	bTimeButton = true;
	TimeButtonActivate();
}
void UMainMenu::TimeButton3_Callback()
{
	time = 90;
	Time_Button_0->SetIsEnabled(true); Time_Button_1->SetIsEnabled(true); Time_Button_2->SetIsEnabled(true); Time_Button_3->SetIsEnabled(false);
	bTimeButton = true;
	TimeButtonActivate();
}