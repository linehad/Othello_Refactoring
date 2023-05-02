// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard.h"
#include "OthelloGameModeBase.h"
#include "OthelloPices_UserWidget.h"
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

	MainMenu_Button->OnClicked.AddDynamic(this, &UGameBoard::GoMainMenu);
	Quit_Button->OnClicked.AddDynamic(this, &UGameBoard::QuitGame);
	ReStart_Button->OnClicked.AddDynamic(this, &UGameBoard::ReStartGame);

	m_Board_Size = Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetBoardSize();
	m_Turn_time = Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetTime();

	// ai 가중치를 저장할 보드 정의
	ai_ReverseIndex.Empty();
	aiBoard.Init(2, m_Board_Size * m_Board_Size);

	// 가중치 설정
	int32 maxSize = m_Board_Size - 1;
	int32 avoidPos = maxSize - 1;

	// 우선순위가 높은 위치	
	int32 aiIndex = 0;
	for (int32 i = 0; i < 2; i++)
	{
		// 가로
		for (int32 j = 1; j < maxSize; j++)
		{
			aiBoard[Board_index(j, aiIndex)] = 9;
		}
		// 세로
		for (int32 k = 1; k < maxSize; k++)
		{
			aiBoard[Board_index(aiIndex, k)] = 9;
		}
		aiIndex = maxSize;
	}

	// 우선순위가 낮은 위치
	aiIndex = 1;
	for (int32 i = 0; i < 2; i++)
	{
		// 가로
		for (int32 j = 2; j < avoidPos; j++)
		{
			aiBoard[Board_index(j, aiIndex)] = 1;
		}
		// 세로
		for (int32 k = 2; k < avoidPos; k++)
		{
			aiBoard[Board_index(aiIndex, k)] = 1;
		}
		aiIndex = avoidPos;
	}

	// 무조건 두어야 하는 위치
	aiBoard[Board_index(0, 0)] = 10;
	aiBoard[Board_index(maxSize, 0)] = 10;
	aiBoard[Board_index(0, maxSize)] = 10;
	aiBoard[Board_index(maxSize, maxSize)] = 10;

	// 무조건 피해야 하는 위치
	aiBoard[Board_index(1, 1)] = 0;
	aiBoard[Board_index(avoidPos, 1)] = 0;
	aiBoard[Board_index(1, avoidPos)] = 0;
	aiBoard[Board_index(avoidPos, avoidPos)] = 0;

	aiBoard[Board_index(1, 0)] = 0;
	aiBoard[Board_index(0, 1)] = 0;
	aiBoard[Board_index(0, avoidPos)] = 0;
	aiBoard[Board_index(1, maxSize)] = 0;

	aiBoard[Board_index(avoidPos, 0)] = 0;
	aiBoard[Board_index(maxSize, 1)] = 0;
	aiBoard[Board_index(maxSize, avoidPos)] = 0;
	aiBoard[Board_index(avoidPos, maxSize)] = 0;

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UGameBoard::CountDown, 1.f, true, 0.0);
	seconds = m_Turn_time;

	Time_Image0->SetBrushFromTexture(BigFont[0], true);
	Time_Image1->SetBrushFromTexture(BigFont[0], true);

	White_Image0->SetBrushFromTexture(BigFont[2], true);
	White_Image1->SetBrushFromTexture(BigFont[0], true);

	Black_Image0->SetBrushFromTexture(BigFont[2], true);
	Black_Image1->SetBrushFromTexture(BigFont[0], true);


	// 버튼에 정보 넘겨주고 함수와 묶기
	if (OthelloButton)
	{
		int arr_index = 0;

		Arr_OthelloButton.Init(nullptr, m_Board_Size * m_Board_Size);
		for (int i = 0; i < m_Board_Size; i++)
		{
			for (int j = 0; j < m_Board_Size; j++)
			{
				UUserWidget* widget = CreateWidget(this, OthelloButton);
				Board_UniformGridPanel->AddChildToUniformGrid(widget, i, j);
				Arr_OthelloButton[arr_index] = Cast<UOthelloPices_UserWidget>(widget);
				Arr_OthelloButton[arr_index]->SetData(i, j);
				Arr_OthelloButton[arr_index]->OthlloPices_Button->OnClicked.AddDynamic(this, &UGameBoard::OthelloNextturn);
				arr_index++;
			}
		}

		// 시작 돌 놓기
		int y = m_Board_Size / 2 - 1;
		int index = 0;
		for (int i = 0; i < 2; i++)
		{
			int x = (m_Board_Size / 2) - 1;
			for (int j = 0; j < 2; j++)
			{
				index = Board_index(x, y);
				Arr_OthelloButton[index]->ReversePice();

				if (i == 0)aiBoard[index] = aiPice[1 - j];
				else aiBoard[index] = aiPice[j];

				OthelloChangeturn();
				x++;
			}
			y++;
			OthelloChangeturn();
		}
		OthelloChangeturn();
	}

}

void UGameBoard::SetTime(int time)
{
	// 점수 출력 부

	Time_Image0->SetBrushFromTexture(BigFont[time % 10], true);
	Time_Image1->SetBrushFromTexture(BigFont[time / 10], true);
}

void UGameBoard::GameOver(int8 GameWinner)
{
	GameOver_GridPanel-> SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (GameWinner == 0)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == 1)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == 2)
	{
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UGameBoard::GoMainMenu()
{
	auto UserWidget = LoadObject<UClass>(NULL, TEXT("/Game/BP_MainMenu.BP_MainMenu_C"), NULL, LOAD_None, NULL);
	Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ChangeMenuWidget(UserWidget);
}

void UGameBoard::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UGameBoard::ReStartGame()
{
	auto UserWidget = LoadObject<UClass>(NULL, TEXT("/Game/BP_Board.BP_Board_C"), NULL, LOAD_None, NULL);
	Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ChangeMenuWidget(UserWidget);
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
			OthelloChangeturn();
		}
		else
		{
			PlayAnimation(Time_Image1_Anim);
		}
		
	}
	if (Arr_OthelloButton[(m_Board_Size * m_Board_Size) - 1]->GetTurn() == true && seconds <= m_Turn_time - 3)
	{
		AiPlayer();
	}
}

void UGameBoard::AiPlayer()
{
	int Indexpos = 0;
	int max = 0;
	int aiBoardIndex = -1;
	for (int i = 0; i < ai_ReverseIndex.Num(); i++)
	{
		int index = ai_ReverseIndex[i].Key;
		// aiBoardIndex의 가중치가 가장 높으며, 두번째 값 즉 뒤집을 수 있는 개수가 가장 큰 놈을 찾고
		if (aiBoardIndex <= aiBoard[index] && max < ai_ReverseIndex[i].Value)
		{
			// 가장 큰 놈의 위치를 저장한다.
			aiBoardIndex = aiBoard[index];
			Indexpos = index;
		}
	}
	// 그리고 해당 위치에 흰 돌을 놓는다.
	aiBoard[Indexpos] = aiPice[1];
	Arr_OthelloButton[Indexpos]->OnBtnClick();
	OthelloNextturn();
}

void UGameBoard::SetScore(int BScore, int WScore)
{
	// 점수 출력 부
	if (m_bScore % 10 != BScore % 10)
	{
		PlayAnimation(Black_Image0_Anim);
		Black_Image0->SetBrushFromTexture(BigFont[BScore % 10], true);
	}

	if (m_bScore / 10 != BScore / 10)
	{
		PlayAnimation(Black_Image1_Anim);
		Black_Image1->SetBrushFromTexture(BigFont[BScore / 10], true);
	}

	if (m_wScore % 10 != WScore % 10)
	{
		PlayAnimation(White_Image0_Anim);
		White_Image0->SetBrushFromTexture(BigFont[WScore % 10], true);
	}
	if (m_wScore / 10 != WScore / 10)
	{
		PlayAnimation(White_Image1_Anim);
		White_Image1->SetBrushFromTexture(BigFont[WScore / 10], true);
	}
}

void UGameBoard::ChangePices(int x, int y, int8 piceColor)
{
	const int direction = 8;

	for (int i = 0; i < direction; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // 자신의 색을 만나면 true 그 사이에 있는 것들을 뒤집는다.

		while (nx >= 0 && ny >= 0 && nx < m_Board_Size && ny < m_Board_Size && Arr_OthelloButton[Board_index(nx, ny)]->GetPice() != 0)
		{
			if (Arr_OthelloButton[Board_index(nx, ny)]->GetPice() == piceColor)
			{
				flag = true;
				break;
			}
			nx += arr_dX[i];
			ny += arr_dY[i];
		}

		while (flag)
		{
			if (nx == x && ny == y)
			{
				break;
			}
			// 실제로 뒤집는다.
			Arr_OthelloButton[Board_index(nx, ny)]->ReversePice();

			// aiBoard에 색돌 정보를 반영한다.
			if (Arr_OthelloButton[Board_index(nx, ny)]->GetPice() == 1)
			{
				aiBoard[Board_index(nx, ny)] = aiPice[0];
			}
			else if (Arr_OthelloButton[Board_index(nx, ny)]->GetPice() == 2)
			{
				aiBoard[Board_index(nx, ny)] = aiPice[1];
			}
			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
}

void UGameBoard::OthelloChangeturn()
{
	ai_ReverseIndex.Empty();
	int wScore = 0;
	int bScore = 0;
	seconds = m_Turn_time;
	SetTime(seconds);

	// 버튼 초기화
	for (int i = 0; i < m_Board_Size * m_Board_Size; i++)
	{
		Arr_OthelloButton[i]->UnPossiblePice();
	}

	for (int i = 0; i < m_Board_Size * m_Board_Size; i++)
	{
		wScore = Arr_OthelloButton[i]->GetPice() == 2 ? ++wScore : wScore;
		bScore = Arr_OthelloButton[i]->GetPice() == 1 ? ++bScore : bScore;

		int32 Px = Arr_OthelloButton[i]->GetX();
		int32 Py = Arr_OthelloButton[i]->GetY();
		Arr_OthelloButton[i]->Changeturn();
		if (PutOthello(Px, Py, Arr_OthelloButton[i]->GetTurn() + 1) && Arr_OthelloButton[i]->GetPice() == 0)
		{
			TPair<int32, int32>AireverseIndex(Board_index(Px, Py), AireverseNum); // 해당 위치에 두었을때 뒤집을 수 있는 돌의 개수와 위치
			ai_ReverseIndex.Add(AireverseIndex);
			Arr_OthelloButton[i]->PossiblePice();
		}
	}
}

void UGameBoard::OthelloNextturn()
{
	int wScore = 0;
	int bScore = 0;
	int PutCount = 0; // 착수 가능한지 센다.
	int gameOver = 0;
	int8 BlackWin = 0;

	int Bcount = 0;
	int Wcount = 0;

	ai_ReverseIndex.Empty();
	seconds = m_Turn_time;
	SetTime(seconds);

	// 버튼 초기화
	for (int i = 0; i < m_Board_Size * m_Board_Size; i++)
	{
		Arr_OthelloButton[i]->UnPossiblePice();
		Bcount = Arr_OthelloButton[i]->GetPice() == 1 ? ++Bcount : Bcount;
		Wcount = Arr_OthelloButton[i]->GetPice() == 2 ? ++Wcount : Wcount;
	}

	if (Bcount == 0)
	{
		GameOver(2);
	}
	else if (Wcount == 0)
	{
		GameOver(1);
	}

	// 배열을 돌면서 눌린 버튼을 찾고 뒤집는다.
	for (int i = 0; i < m_Board_Size * m_Board_Size; i++)
	{
		if (Arr_OthelloButton[i]->b_hit)
		{
			ChangePices(Arr_OthelloButton[i]->GetX(), Arr_OthelloButton[i]->GetY(), Arr_OthelloButton[i]->GetPice());
			Arr_OthelloButton[i]->b_hit = false;
			break;
		}
	}


	// 순서를 바꾸고, 누를 수 있는 위치를 다시 표시하며 점수를 센다.
	// 게임 둘 수 있는 위치가 있는지 체크하고 없으면 턴을 넘긴다.
	// 게임이 끝났는지 체크한다.
	for (int i = 0; i < m_Board_Size * m_Board_Size; i++)
	{
		wScore = Arr_OthelloButton[i]->GetPice() == 2 ? ++wScore : wScore;
		bScore = Arr_OthelloButton[i]->GetPice() == 1 ? ++bScore : bScore;
		gameOver = Arr_OthelloButton[i]->GetPice() == 0 ? ++gameOver : gameOver;

		int32 Px = Arr_OthelloButton[i]->GetX();
		int32 Py = Arr_OthelloButton[i]->GetY();
		Arr_OthelloButton[i]->Changeturn();
		// 해당 영역에 둘수 있는지 없는지를 체크한다.
		if (PutOthello(Px, Py, Arr_OthelloButton[i]->GetTurn() + 1) && Arr_OthelloButton[i]->GetPice() == 0)
		{
			TPair<int32, int32>AireverseIndex(Board_index(Px, Py), AireverseNum); // 해당 위치에 두었을때 뒤집을 수 있는 돌의 개수와 위치
			ai_ReverseIndex.Add(AireverseIndex);
			Arr_OthelloButton[i]->PossiblePice();
			PutCount++;
		}
	}


	SetScore(bScore, wScore);
	m_bScore = bScore;
	m_wScore = wScore;

	if (gameOver == 0)
	{
		// 같으면 0 검은색이 이기면 1 흰색이 이기면 2
		BlackWin = bScore == wScore ? 0 : (bScore > wScore) ? 1 : 2;
		GameOver(BlackWin);
	}
	// 더이상 누를데가 없다면 턴을 뒤집는다.
	else if (PutCount == 0)
	{
		OthelloChangeturn();
	}
}

// 둘수 있는 위치를 표시해주는 함수
bool UGameBoard::PutOthello(int x, int y, int8 piceColor)
{
	AireverseNum = 0;
	for (int i = 0; i < 8; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		while (nx >= 0 && ny >= 0 && nx < m_Board_Size && ny < m_Board_Size 
			&& Arr_OthelloButton[Board_index(nx, ny)]->GetPice() != 0)
		{
			AireverseNum++;

			// 바로 다음돌이 내 색이랑 같을 경우
			if (piceColor == Arr_OthelloButton[Board_index(x + arr_dX[i], y + arr_dY[i])]->GetPice())
			{
				break;
			}
			else if (Arr_OthelloButton[Board_index(nx, ny)]->GetPice() == piceColor)
			{
				return true;
			}
			nx += arr_dX[i];
			ny += arr_dY[i];
		}
	}
	return false;
}
