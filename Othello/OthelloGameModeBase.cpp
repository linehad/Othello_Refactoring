#include "OthelloGameModeBase.h"
#include "ServerGameStateBase.h"
#include "GameBoard.h"
#include "OthelloPices_UserWidget.h"
#include "OthelloNativeClass.h"
#include "OthelloPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>

void AOthelloGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOthelloGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(NewPlayer);

		if (Controller)
		{
			Controller->ChangeWidget(StartingWidgetClass);
		}
		if(GetWorld()->GetNumPlayerControllers() == 2)
		{		
			Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->ChangeWidget(MainWidgetClass);
		}
}

void AOthelloGameModeBase::SetGameData(int size, int time)
{
	limitTime = time;
	boardSize = size;
	arrOthelloButton.Empty();
	arrOthelloButton.Init(EMPTY, size);

	Cast<AServerGameStateBase>(GetWorld()->GetGameState())->SetData(time, size);
	for (auto Iter = GetWorld()->GetControllerIterator(); Iter; ++Iter)
	{
		Cast<AOthelloPlayerController>(Iter->Get())->ResetBoardWidget(BoardWidgetClass, size, time);
	}
}

void AOthelloGameModeBase::SetOthelloNextturn_Implementation()
{
	AOthelloPlayerController* ServerController = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());
	UGameBoard* OthelloBoard = Cast<UGameBoard>(ServerController->CurrentWidget);

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter) {
		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(Iter->Get());
		if (Controller) {
			Controller->OthelloNextturn(othelloArrIndex);
		}
	}
}

void AOthelloGameModeBase::OthelloNextTurn(int othelloArrIndex)
{
	AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());
	UGameBoard* OthelloBoard = Cast<UGameBoard>(Controller->CurrentWidget);
	OthelloNativeClass logic;
	int white_score = 0;
	int black_score = 0;
	int PutCount = 0; // 착수 가능한지 센다.
	int gameOver = 0;
	int8 winnner = 0;

	int black_count = 0;
	int white_count = 0;

	const int DRAW = 0;
	const int BLACK = 1;
	const int WHITE = 2;

	OthelloBoard->SetTime(limitTime);

	// 버튼 초기화
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		OthelloBoard->arrOthelloButton[i]->UnPossiblePiece();
		black_count = OthelloBoard->arrOthelloButton[i]->GetPiece() == BLACK ? ++black_count : black_count;
		white_count = OthelloBoard->arrOthelloButton[i]->GetPiece() == WHITE ? ++white_count : white_count;
	}

	if (black_count == 0)
	{
		OthelloBoard->GameOver(2);
	}
	else if (white_count == 0)
	{
		OthelloBoard->GameOver(1);
	}

	// 버튼을 찾고 뒤집는다.
	OthelloBoard->arrOthelloButton[othelloArrIndex]->OnBtnClick();
	logic.ChangePices(OthelloBoard->arrOthelloButton[othelloArrIndex]->GetX(),
		OthelloBoard->arrOthelloButton[othelloArrIndex]->GetY(), OthelloBoard->arrOthelloButton[othelloArrIndex]->GetPiece(), boardSize, OthelloBoard->arrOthelloButton);

	// 클라 턴일때도 서버에서 실행됨... 따라서 당장 hit를 초기화 하는게 아니라 다음번 순서에 초기화 해야함...
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		if (bcheckTurn)
		{
			OthelloBoard->arrOthelloButton[beforeIndex]->bhit = false;
			bcheckTurn = false;
		}
		else
		{
			bcheckTurn = true;
		}
	}
	beforeIndex = othelloArrIndex;
	if (GetWorld()->GetFirstPlayerController()->GetRemoteRole() == ROLE_Authority)
	{
		OthelloBoard->arrOthelloButton[othelloArrIndex]->bhit = false;
	}

	// 순서를 바꾸고, 누를 수 있는 위치를 다시 표시하며 점수를 센다.
	// 게임 둘 수 있는 위치가 있는지 체크하고 없으면 턴을 넘긴다.
	// 게임이 끝났는지 체크한다.
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		white_score = OthelloBoard->arrOthelloButton[i]->GetPiece() == WHITE ? ++white_score : white_score;
		black_score = OthelloBoard->arrOthelloButton[i]->GetPiece() == BLACK ? ++black_score : black_score;
		gameOver = OthelloBoard->arrOthelloButton[i]->GetPiece() == 0 ? ++gameOver : gameOver;

		int32 Px = OthelloBoard->arrOthelloButton[i]->GetX();
		int32 Py = OthelloBoard->arrOthelloButton[i]->GetY();
		OthelloBoard->arrOthelloButton[i]->ChangeTurn();
		// 해당 영역에 둘수 있는지 없는지를 체크한다.
		if (logic.PutOthello(Px, Py, OthelloBoard->arrOthelloButton[i]->GetTurn() + 1, boardSize, OthelloBoard->arrOthelloButton)
			&& OthelloBoard->arrOthelloButton[i]->GetPiece() == 0)
		{
			// 해당 위치에 두었을때 뒤집을 수 있는 돌의 개수와 위치
			OthelloBoard->arrOthelloButton[i]->PossiblePiece();
			PutCount++;
		}
	}


	OthelloBoard->SetScore(black_score, white_score);
	OthelloBoard->SetblackScore(black_score);
	OthelloBoard->SetwhiteScore(white_score);

	if (changeCount > 2)
	{
		winnner = black_score == white_score ? DRAW : (black_score > white_score) ? BLACK : WHITE;
		OthelloBoard->GameOver(winnner);
	}

	if (gameOver == 0)
	{
		// 같으면 0 검은색이 이기면 1 흰색이 이기면 2
		winnner = black_score == white_score ? DRAW : (black_score > white_score) ? BLACK : WHITE;
		OthelloBoard->GameOver(winnner);
	}
	// 더이상 누를데가 없다면 턴을 뒤집는다.
	else if (PutCount == 0)
	{
		changeCount++;
		OthelloChangeTurn();
	}
}

void AOthelloGameModeBase::OthelloChangeTurn()
{
	int white_score = 0;
	int black_score = 0;
	OthelloNativeClass logic;
	//seconds = limitTime;
	//SetTime(seconds); // 타이머 위젯 하나 만들것

	// 버튼 초기화
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		arrOthelloButton[i]->UnPossiblePiece(); // 개문제 그냥
	}

	for (int i = 0; i < boardSize * boardSize; i++)
	{
		white_score = arrOthelloButton[i]->GetPice() == 2 ? ++white_score : white_score;
		black_score = arrOthelloButton[i]->GetPice() == 1 ? ++black_score : black_score;

		int32 pX = arrOthelloButton[i]->GetX();
		int32 pY = arrOthelloButton[i]->GetY();
		arrOthelloButton[i]->Changeturn();
		if (logic.PutOthello(pX, pY, arrOthelloButton[i]->GetTurn() + 1, boardSize, arrOthelloButton) && arrOthelloButton[i]->GetPice() == 0)
		{
			arrOthelloButton[i]->PossiblePice();
		}
	}
}