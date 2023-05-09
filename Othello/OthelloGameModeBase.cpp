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
	if (GetWorld()->GetNumPlayerControllers() == 2)
	{
		Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->ChangeWidget(MainWidgetClass);
	}
}

void AOthelloGameModeBase::SetGameData(int size, int time)
{
	GameInfoStruct.Time = time;
	GameInfoStruct.Size = size * size;
	GameInfoStruct.GameTurn = BLACK_TURN; // 나중에 게임을 다시 시작하게 될 경우를 위한 초기화

	othelloBoard.Empty();
	othelloBoard.Init(EMPTY, size);

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

// OthelloChangeTurn과 다를게 없어 보이는데 없애도 상관 없어 보인다.
void AOthelloGameModeBase::OthelloNextTurn(int arrIndex)
{
	AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());
	//UGameBoard* OthelloBoard = Cast<UGameBoard>(Controller->CurrentWidget);
	OthelloNativeClass logic;
	int white_score = 0; // 점수는 스테이트로 옮기는게 좋아 보인다.
	int black_score = 0;
	int PutCount = 0; // 몇 개나 착수 가능한지 세는 변수 0개면 상대에게 턴이 넘어감
	int gameOver = 0;
	int8 winnner = 0;

	int32 black_count = 0;
	int32 white_count = 0;

	// OthelloBoard->SetTime(limitTime); 타미머 돌리는 함수 바꿔주자

	// 버튼 초기화
	for (int i = 0; i < GameInfoStruct.Size; i++)
	{
		// OthelloBoard->arrOthelloButton[i]->UnPossiblePiece(); // 누를 수 있던 부분을 비활성화 해주는 부분 보드로 옮기자
		white_score = othelloBoard[i] == WHITE_PIECE ? ++white_score : white_score; // 점수를 세는 기능들
		black_score = othelloBoard[i] == BLACK_PIECE ? ++black_score : black_score;
	}

	//if (black_count == 0)
	//{
	//	OthelloBoard->GameOver(WHITE_PIECE);
	//}
	//else if (white_count == 0)
	//{
	//	OthelloBoard->GameOver(BLACK_PIECE);
	//}

	// 버튼을 찾고 뒤집는다. 이건 보드로 가야 된다.
	//OthelloBoard->arrOthelloButton[othelloArrIndex]->OnBtnClick();
	//logic.ChangePices(OthelloBoard->arrOthelloButton[othelloArrIndex]->GetX(),
	//	OthelloBoard->arrOthelloButton[othelloArrIndex]->GetY(), 
	//	OthelloBoard->arrOthelloButton[othelloArrIndex]->GetPiece(), 
	//	OthelloBoard->arrOthelloButton);

	// 클라 턴일때도 서버에서 실행됨... 따라서 당장 hit를 초기화 하는게 아니라 다음번 순서에 초기화 해야함...
	// 이 부분은 너무 억지로 만든 느낌이다. 다른 방법을 생각해보자
	/*if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
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
	}*/

	// 순서를 바꾸고, 누를 수 있는 위치를 다시 표시하며 점수를 센다.
	// 게임 둘 수 있는 위치가 있는지 체크하고 없으면 턴을 넘긴다.
	// 게임이 끝났는지 체크한다.

	// 아래와 중복되는 for문이다. 
	//for (int i = 0; i < boardSize * boardSize; i++)
	//{
	//	white_score = othelloBoard[i] == WHITE_PIECE ? ++white_score : white_score; // 점수를 세는 기능들
	//	black_score = othelloBoard[i] == BLACK_PIECE ? ++black_score : black_score;
	//	gameOver = othelloBoard[i] == EMPTY ? ++gameOver : gameOver; // 비어있는 곳이 있다면 증가

	//	int32 Px = OthelloBoard->arrOthelloButton[i]->GetX();
	//	int32 Py = OthelloBoard->arrOthelloButton[i]->GetY();
	//	OthelloBoard->arrOthelloButton[i]->ChangeTurn();
	//	// 해당 영역에 둘수 있는지 없는지를 체크한다.
	//	if (logic.IsPutOthello(Px, Py, gameTurn, OthelloBoard)
	//		&& OthelloBoard->arrOthelloButton[i]->GetPiece() == 0)
	//	{
	//		// 둘 수 있으면 착수 가능한 상태로 바꿈
	//		OthelloBoard->arrOthelloButton[i]->PossiblePiece();
	//		PutCount++;
	//	}
	//}

	// 점수는 위젯을 하나 새로 구현하자.
	//OthelloBoard->SetScore(black_score, white_score);
	//OthelloBoard->SetblackScore(black_score);
	//OthelloBoard->SetwhiteScore(white_score);

	// 승리 위젯도 마찬가지
	//if (changeCount > 2)
	//{
	//	winnner = black_score == white_score ? DRAW : (black_score > white_score) ? BLACK : WHITE;
	//	OthelloBoard->GameOver(winnner);
	//}

	//if (gameOver == 0)
	//{
	//	// 같으면 0 검은색이 이기면 1 흰색이 이기면 2
	//	winnner = black_score == white_score ? DRAW : (black_score > white_score) ? BLACK : WHITE;
	//	OthelloBoard->GameOver(winnner);
	//}
	//// 더이상 누를데가 없다면 턴을 뒤집는다.
	//else if (PutCount == 0)
	//{
	//	changeCount++;
	//	OthelloChangeTurn();
	//}
}

void AOthelloGameModeBase::OthelloChangeTurn()
{
	GameInfoStruct.GameTurn = !GameInfoStruct.GameTurn; // not 연산으로 1은 0으로 0은 1로 바뀜 사실상 bool형 변수
}

TArray<int8> AOthelloGameModeBase::OthelloChangeTurn(int32 pX, int32 pY)
{
	int white_score = 0;
	int black_score = 0;
	OthelloNativeClass logic;
	TArray<int8> possiblePos;
	possiblePos.Empty();

	GameInfoStruct.GameTurn = !GameInfoStruct.GameTurn; // not 연산으로 1은 0으로 0은 1로 바뀜 사실상 bool형 변수
	//seconds = limitTime;
	//SetTime(seconds); // 타이머 위젯 하나 만들것

	// 여기서 누를 곳을 표시할 위치를 계산 해야함

	// 버튼 초기화
	//for (int i = 0; i < boardSize * boardSize; i++)
	//{
	//	arrOthelloButton[i]->UnPossiblePiece(); // 개문제 모든 배열을 돌면서 버튼 초기화 이 부분은 보드에서 처리
	//}

	for (int i = 0; i < GameInfoStruct.Size; i++)
	{
		white_score = othelloBoard[i] == WHITE_PIECE ? ++white_score : white_score; // 점수를 세는 기능들
		black_score = othelloBoard[i] == BLACK_PIECE ? ++black_score : black_score;

		//int32 pX = arrOthelloButton[i]->GetX(); // 이건 또 뭐야
		//int32 pY = arrOthelloButton[i]->GetY();
		// othelloBoard[i]->Changeturn(); // 이놈도 각 버튼의 턴을 바꿔주는데 보드에서 구현 해야함
		if (logic.IsPutOthello(pX, pY, GameInfoStruct.GameTurn, othelloBoard) && othelloBoard[i] == EMPTY) // 착수 가능한 위치를 표시해주는 기능
		{
			// othelloBoard[i]->PossiblePice();	 // 마찬가지로 보드에 구현할것 여기서는 리턴만 해줄거임
			possiblePos.Add(i);
		}
	}
	return possiblePos;
}
