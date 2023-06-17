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
	if (GameState) // 스테이트에서 전달해주는 구조체를 통해 착수이벤트 바인딩
	{
		GameState->OnPlacementEvent.AddDynamic(this, &UScoreUserWidget::Placement);
	}
	// 텍스쳐 매핑
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
	if (EventName == TEXT("Placement")) // 착수한 경우
	{
		// 시간을 초기화하고 애니매이션 재생함
		seconds = timeLimit;
		SetTime(seconds);
		PlayAnimation(Time_Image0_Anim);
		PlayAnimation(Time_Image1_Anim);

		// 점수를 설정함
		SetScore(Data.blackScore, Data.whiteScore);

		// 점수를 저장함
		lastWhiteScore = Data.whiteScore;
		lastBlackScore = Data.blackScore;

	}
	else if (EventName == TEXT("GameOver")) // 게임이 끝난 경우
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle); // 타이머 이벤트를 해제시킨다.

		if (Data.blackScore > Data.whiteScore) // 흑돌이 이겼을 경우
		{
			GameOver(BLACK_WIN);
		}
		else if (Data.blackScore < Data.whiteScore) // 백돌이 이겼을 경우
		{
			GameOver(WHITE_WIN);
		}
		else // 무승부
		{
			GameOver(DRAW);
		}
	}
}

void UScoreUserWidget::SetScore(int black_score, int white_score)
{
	// 점수 출력 부
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

	// 타이머 핸들러를 통해 시간을 업데이트한다.
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

			// 타임아웃 처리는 여기서 함 브로드캐스트로 받아내면 너무 느리기 때문
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