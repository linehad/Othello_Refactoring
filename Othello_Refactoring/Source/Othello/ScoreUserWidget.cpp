// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreUserWidget.h"
#include "ServerGameStateBase.h"
#include "OthelloPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Components/TextBlock.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/GridPanel.h>
#include <Components/UniformGridPanel.h>
#include "Animation/WidgetAnimation.h"

void UScoreUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Quit_Button->OnClicked.AddDynamic(this, &UScoreUserWidget::QuitGame);

	GameState = Cast<AServerGameStateBase>(GetWorld()->GetGameState());
	if (GameState) // ������Ʈ���� �������ִ� ����ü�� ���� �����̺�Ʈ ���ε�
	{
		GameState->OnPlacementEvent.AddDynamic(this, &UScoreUserWidget::Placement);
	}
	// �ؽ��� ����
	BigFont.Empty();
	for (int i = 0; i < 10; i++)
	{
		FString a = ".";
		FString BigFontAddress = TEXT("/Game/Font/BigFont/") + FString::FromInt(i) + a + FString::FromInt(i);
		BigFont.Add(LoadObject<UTexture2D>(NULL, *BigFontAddress, NULL, LOAD_None, NULL));
	}

	Time_Image0->SetBrushFromTexture(BigFont[0], true);
	Time_Image1->SetBrushFromTexture(BigFont[0], true);

	White_Image0->SetBrushFromTexture(BigFont[2], true);
	White_Image1->SetBrushFromTexture(BigFont[0], true);

	Black_Image0->SetBrushFromTexture(BigFont[2], true);
	Black_Image1->SetBrushFromTexture(BigFont[0], true);
}

void UScoreUserWidget::Placement(FName EventName, const FBoardInfoStruct& Data)
{
	if (EventName == TEXT("Placement")) // ������ ���
	{
		// �ð��� �ʱ�ȭ�ϰ� �ִϸ��̼� �����
		seconds = timeLimit;
		SetTime(seconds);
		PlayAnimation(Time_Image0_Anim);
		PlayAnimation(Time_Image1_Anim);

		// ������ ������
		SetScore(Data.blackScore, Data.whiteScore);

		// ������ ������
		lastWhiteScore = Data.whiteScore;
		lastBlackScore = Data.blackScore;

	}
	else if (EventName == TEXT("GameOver")) // ������ ���� ���
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle); // Ÿ�̸� �̺�Ʈ�� ������Ų��.

		if (Data.blackScore > Data.whiteScore) // �浹�� �̰��� ���
		{
			GameOver(BLACK_WIN);
		}
		else if (Data.blackScore < Data.whiteScore) // �鵹�� �̰��� ���
		{
			GameOver(WHITE_WIN);
		}
		else // ���º�
		{
			GameOver(DRAW);
		}
	}
}

void UScoreUserWidget::SetScore(int black_score, int white_score)
{
	// ���� ��� ��
	if (lastBlackScore % 10 != black_score % 10)
	{
		PlayAnimation(Black_Image0_Anim);
		Black_Image0->SetBrushFromTexture(BigFont[black_score % 10], true);
	}

	if (lastBlackScore / 10 != black_score / 10)
	{
		PlayAnimation(Black_Image1_Anim);
		Black_Image1->SetBrushFromTexture(BigFont[black_score / 10], true);
	}

	if (lastWhiteScore % 10 != white_score % 10)
	{
		PlayAnimation(White_Image0_Anim);
		White_Image0->SetBrushFromTexture(BigFont[white_score % 10], true);
	}
	if (lastWhiteScore / 10 != white_score / 10)
	{
		PlayAnimation(White_Image1_Anim);
		White_Image1->SetBrushFromTexture(BigFont[white_score / 10], true);
	}
}

void UScoreUserWidget::SetTime(int time)
{
	Time_Image0->SetBrushFromTexture(BigFont[time % 10], true);
	Time_Image1->SetBrushFromTexture(BigFont[time / 10], true);
}

void UScoreUserWidget::StartTimer()
{
	GameInfoStruct = GameState->GetGameInfoStruct();
	timeLimit = GameInfoStruct.Time;
	seconds = timeLimit;

	// Ÿ�̸� �ڵ鷯�� ���� �ð��� ������Ʈ�Ѵ�.
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UScoreUserWidget::CountDown, 1.f, true, 0.0);
}

void UScoreUserWidget::CountDown()
{
	bool flag = false;
	if (seconds != 0)
	{
		seconds--;
		SetTime(seconds);
		PlayAnimation(Time_Image0_Anim);
	}
	if (seconds % 10 == 0)
	{
		if (seconds == 0)
		{
			if (GetWorld()->GetFirstPlayerController()->HasAuthority())
			{
				Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->Server_TimeOut();
			}

			// Ÿ�Ӿƿ� ó���� ���⼭ �� ��ε�ĳ��Ʈ�� �޾Ƴ��� �ʹ� ������ ����
			seconds = timeLimit;
			SetTime(seconds);
			PlayAnimation(Time_Image0_Anim);
			PlayAnimation(Time_Image1_Anim);
		}
		else
		{
			PlayAnimation(Time_Image1_Anim);
		}

	}
}

void UScoreUserWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}


void UScoreUserWidget::GameOver(int8 GameWinner)
{
	GameOver_GridPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (GameWinner == DRAW)
	{
		Draw_Text->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == BLACK_WIN)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		Win_Text->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == WHITE_WIN)
	{
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		Win_Text->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}