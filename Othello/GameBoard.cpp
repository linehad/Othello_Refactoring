#include "GameBoard.h"
#include "OthelloPlayerController.h"

#include "OthelloPices_UserWidget.h"
#include "OthelloNativeClass.h"

#include "Kismet/GameplayStatics.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/GridPanel.h>
#include <Components/UniformGridPanel.h>
#include "Animation/WidgetAnimation.h"

void UGameBoard::NativeConstruct()
{
	Super::NativeConstruct();

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

void UGameBoard::SetTime(int time)
{
	Time_Image0->SetBrushFromTexture(BigFont[time % 10], true);
	Time_Image1->SetBrushFromTexture(BigFont[time / 10], true);
}

void UGameBoard::GameOver(int8 GameWinner)
{
	Quit_Button->OnClicked.AddDynamic(this, &UGameBoard::QuitGame);

	GameOver_GridPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (GameWinner == DRAW)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == BLACK)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == WHITE)
	{
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UGameBoard::StartTimer()
{
	// 타이머 핸들러를 통해 시간을 업데이트한다.
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UGameBoard::CountDown, 1.f, true, 0.0);
	seconds = limitTime;
}

void UGameBoard::CountDown()
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
			//OthelloChangeTurn();
		}
		else
		{
			PlayAnimation(Time_Image1_Anim);
		}

	}
}

void UGameBoard::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}


void UGameBoard::NextTurn()
{
	//Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->SetOthelloNextTurn();
}

void UGameBoard::SetScore(int black_score, int white_score)
{
	// 점수 출력 부
	if (beforBlackScore % 10 != black_score % 10)
	{
		PlayAnimation(Black_Image0_Anim);
		Black_Image0->SetBrushFromTexture(BigFont[black_score % 10], true);
	}

	if (beforBlackScore / 10 != black_score / 10)
	{
		PlayAnimation(Black_Image1_Anim);
		Black_Image1->SetBrushFromTexture(BigFont[black_score / 10], true);
	}

	if (beforWhiteScore % 10 != white_score % 10)
	{
		PlayAnimation(White_Image0_Anim);
		White_Image0->SetBrushFromTexture(BigFont[white_score % 10], true);
	}
	if (beforWhiteScore / 10 != white_score / 10)
	{
		PlayAnimation(White_Image1_Anim);
		White_Image1->SetBrushFromTexture(BigFont[white_score / 10], true);
	}
}

void UGameBoard::StartSet(int32 size, int32 time)
{
	boardSize = size;
	limitTime = time;

	seconds = limitTime;
	// 버튼에 정보 넘겨주고 함수와 묶기
	if (OthelloButton)
	{
		int arr_index = 0;

		arrOthelloButton.Init(nullptr, boardSize * boardSize);
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				UUserWidget* widget = CreateWidget(this, OthelloButton);
				Board_UniformGridPanel->AddChildToUniformGrid(widget, i, j);
				arrOthelloButton[arr_index] = Cast<UOthelloPices_UserWidget>(widget);
				arrOthelloButton[arr_index]->SetData(i, j);
				arrOthelloButton[arr_index]->OthlloPiece_Button->OnClicked.AddDynamic(this, &UGameBoard::NextTurn);
				arr_index++;
			}
		}
	}

	// 가운데 돌 두는 기능
	int y = boardSize / 2 - 1;
	int x = (boardSize / 2) - 1;
	int index = 0;

	for (int i = 0; i < 2; i++)
	{
		index = Board_index(x++, y++);
		arrOthelloButton[index]->ReversePiece(); // 백돌 놓음
	}
	for (int i = 0; i < 2; i++)
	{
		y = boardSize / 2 - 1;
		x = (boardSize / 2);
		index = Board_index(x--, y++);
		arrOthelloButton[index]->SetGameTurn(BLACK_TURN); // 각돌에는 턴을 세는 변수가 있어서 바꿔주어야 함
		arrOthelloButton[index]->ReversePiece(); // 흑돌 놓음
		arrOthelloButton[index]->SetGameTurn(WHITE_TURN); // 바꾼 턴 원위치
	}

}