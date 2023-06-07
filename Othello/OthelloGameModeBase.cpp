#include "OthelloGameModeBase.h"
#include "GameBoard.h"
#include "OthelloPices_UserWidget.h"
#include "OthelloNativeClass.h"
#include "OthelloPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>

void AOthelloGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	othelloBoard.Empty();
	BoardInfoStruct.PlacementPosArr.Empty();
	BoardInfoStruct.Turn;
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
	OthelloNativeClass logic;

	GameInfoStruct.Time = time;
	GameInfoStruct.Size = size; // 나중에 게임을 다시 시작하게 될 경우를 위한 초기화
	//gameTurn = BLACK_TURN;

	othelloBoard.Init(EMPTY, size * size);

	int y = GameInfoStruct.Size / 2;
	int x = (GameInfoStruct.Size / 2 - 1);

	// 보드 맨 처음 세팅
	int index = logic.Board_index(x, y, size);
	othelloBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y - 1, size);
	othelloBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y, size);
	othelloBoard[index] = WHITE_PIECE;
	index = logic.Board_index(x, y - 1, size);
	othelloBoard[index] = WHITE_PIECE;

	OnGameInfoUpdated.Broadcast(TEXT("GameInfo"), GameInfoStruct); // 이벤트 브로드캐스트 해당 이벤트를 구독하는 클레스에 데이터를 전달함
	//Cast<AServerGameStateBase>(GetWorld()->GetGameState())->SetData(time, size);
	for (auto Iter = GetWorld()->GetControllerIterator(); Iter; ++Iter)
	{
		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(Iter->Get());
		Controller->ResetBoardWidget(BoardWidgetClass, size, time);
		Controller->AddScoreWidget(ScoreWidgetClass);
	}

	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	OnTurnUpdated.Broadcast(TEXT("SetStart"), BoardInfoStruct);

}

void AOthelloGameModeBase::ReverseTurn()
{
	BoardInfoStruct.Turn = !BoardInfoStruct.Turn;
}

void AOthelloGameModeBase::Placement(int32 pX, int32 pY)
{
	AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());

	BoardInfoStruct.PlacementPosArr.Empty();

	int whiteScore = 0;
	int blackScore = 0;
	int8 othelloPiece = 0;
	OthelloNativeClass logic;
	TArray<int32> ReversePos;

	const int32 CLICK_POS = logic.Board_index(pX, pY, GameInfoStruct.Size);
	BoardInfoStruct.Index = CLICK_POS;

	// 현재 턴이 검정돌 턴이면 보드에 검정돌을 기록 아니라면 하얀돌을 기록한다.
	othelloPiece = BoardInfoStruct.Turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;
	othelloBoard[CLICK_POS] = othelloPiece;

	// 뒤집어야 하는 돌을 계산하고 보드에 기록한다.
	ReversePos = logic.ChangePices(pX, pY, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	for (int i = 0; i < ReversePos.Num(); i++)
	{
		int32 index = ReversePos[i];
		othelloBoard[index] = othelloPiece;
	}
	ReverseTurn(); // 턴 뒤집음

	// 점수를 세준다.
	for (int32 i = 0; i < GameInfoStruct.Size* GameInfoStruct.Size; i++)
	{
		whiteScore = othelloBoard[i] == WHITE_PIECE ? ++whiteScore : whiteScore;
		blackScore = othelloBoard[i] == BLACK_PIECE ? ++blackScore : blackScore;
	}
	BoardInfoStruct.whiteScore = whiteScore;
	BoardInfoStruct.blackScore = blackScore;

	// 뒤집어야 할 돌을 대입하고 착수 가능한 위치를 계산한다.
	BoardInfoStruct.ReversePosArr = ReversePos;
	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);

	// 더 이상 둘 수 있는 곳이 없다면 턴을 뒤집어서 다시 계산한다.
	if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
	{
		ReverseTurn();
		BoardInfoStruct.PlacementPosArr.Empty();
		BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
		// 턴을 뒤집었는데도 둘 수 있는 곳이 없다면 게임을 종료한다.
		if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
		{
			// 뒤집어야 할 것들이 남았다면 뒤집자.
			if (ReversePos.Num() != EMPTY)
			{
				ReverseTurn();
				OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
			}
			OnTurnUpdated.Broadcast(TEXT("GameOver"), BoardInfoStruct);
		}
		// 아니라면 턴만 뒤집는다.
		else
		{
			OnTurnUpdated.Broadcast(TEXT("TurnOut"), BoardInfoStruct);
		}
	}
	else
	{
		// BoardInfoStruct를 브로드캐스트 하여 해당 이벤트를 구독하는 다른 곳에서 사용가능하게 만든다.
		OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
	}
}

void AOthelloGameModeBase::TimeOut()
{
	OthelloNativeClass logic;
	BoardInfoStruct.PlacementPosArr.Empty();

	BoardInfoStruct.Index = -1;
	BoardInfoStruct.ReversePosArr.Empty();
	ReverseTurn();
	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);

	if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
	{
		ReverseTurn();
		BoardInfoStruct.PlacementPosArr.Empty();
		BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
		if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
		{
			OnTurnUpdated.Broadcast(TEXT("GameOver"), BoardInfoStruct);
		}
		else
		{
			OnTurnUpdated.Broadcast(TEXT("TurnOut"), BoardInfoStruct);
		}
	}
	else
	{
		// BoardInfoStruct를 브로드캐스트 하여 해당 이벤트를 구독하는 다른 곳에서 사용가능하게 만든다.
		OnTurnUpdated.Broadcast(TEXT("TimeOut"), BoardInfoStruct);
	}
}



