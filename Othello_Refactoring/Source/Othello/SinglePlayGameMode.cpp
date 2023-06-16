// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayGameMode.h"
#include "GameBoard.h"
#include "OthelloPices_UserWidget.h"
#include "OthelloCalculation.h"
#include "OthelloPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>

void ASinglePlayGameMode::BeginPlay()
{
	Super::BeginPlay();
	othelloBoard.Empty();
	BoardInfoStruct.PlacementPosArr.Empty();
	BoardInfoStruct.Turn;

	ChangeWidget(MainWidgetClass);
}

void ASinglePlayGameMode::SetGameData(int size, int time)
{
	OthelloCalculation logic;

	GameInfoStruct.Time = time;
	GameInfoStruct.Size = size; // 나중에 게임을 다시 시작하게 될 경우를 위한 초기화

	othelloBoard.Empty();
	othelloBoard.Init(EMPTY, size * size);
	SetAi();

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

	for (auto Iter = GetWorld()->GetControllerIterator(); Iter; ++Iter)
	{
		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(Iter->Get());
		Controller->ResetBoardWidget(BoardWidgetClass, size, time);
		Controller->AddScoreWidget(ScoreWidgetClass);
	}

	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	OnTurnUpdated.Broadcast(TEXT("SetStart"), BoardInfoStruct);

}

void ASinglePlayGameMode::ReverseTurn()
{
	BoardInfoStruct.Turn = !BoardInfoStruct.Turn;
}

void ASinglePlayGameMode::Placement(int32 pX, int32 pY)
{

	AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());

	BoardInfoStruct.PlacementPosArr.Empty();

	int whiteScore = 0;
	int blackScore = 0;
	OthelloCalculation logic;

	// x, y좌표를 1차원 배열로 변환한다.
	BoardInfoStruct.Index = logic.Board_index(pX, pY, GameInfoStruct.Size);

	// 뒤집어야 하는 위치를 계산한다.
	BoardInfoStruct.ReversePosArr = logic.ChangePices(pX, pY, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);

	// 계산된 돌들을 보드에 기록한다.
	RecordPiece(BoardInfoStruct.Index, BoardInfoStruct.ReversePosArr);
	ReverseTurn(); // 턴 뒤집음

	// 점수를 세준다.
	for (int32 i = 0; i < GameInfoStruct.Size * GameInfoStruct.Size; i++)
	{
		whiteScore = othelloBoard[i] == WHITE_PIECE ? ++whiteScore : whiteScore;
		blackScore = othelloBoard[i] == BLACK_PIECE ? ++blackScore : blackScore;
	}
	BoardInfoStruct.whiteScore = whiteScore;
	BoardInfoStruct.blackScore = blackScore;

	// 착수 가능한 위치를 계산한다.
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
			// 뒤집어야 할 것들이 남았다면 턴을 원래대로 바꾸고 돌을 뒤집는다.
			if (BoardInfoStruct.ReversePosArr.Num() != EMPTY)
			{
				ReverseTurn();
				// 뒤집어야 하므로 착수 이벤트 브로드캐스트
				OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
			}

			// 게임오버 브로드캐스트
			OnTurnUpdated.Broadcast(TEXT("GameOver"), BoardInfoStruct);
		}
		// 아니라면 턴만 뒤집는다.
		else
		{
			OnTurnUpdated.Broadcast(TEXT("TurnOut"), BoardInfoStruct);
			GetWorld()->GetTimerManager().SetTimer(AiTimer, this, &ASinglePlayGameMode::AiPlacement, 2.f, false, 1.0);
		}
	}
	else
	{
		// BoardInfoStruct를 브로드캐스트 하여 해당 이벤트를 구독하는 다른 곳에서 사용가능하게 만든다.
		OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
		GetWorld()->GetTimerManager().SetTimer(AiTimer, this, &ASinglePlayGameMode::AiPlacement, 2.f, false, 1.0);
	}
}

void ASinglePlayGameMode::AiPlacement()
{
	OthelloCalculation logic;
	int whiteScore = 0;
	int blackScore = 0;

	int indexPos = -1;
	int bestPos = -1;
	int maxRevers = -1;
	int aiWeight = -1;
	for (int i = 0; i < BoardInfoStruct.PlacementPosArr.Num(); i++)
	{
		int reversIndex = BoardInfoStruct.PlacementPosArr[i]; // 뒤집을 수 있는 위치를 저장한다.
		// 뒤집을 수 있는 개수를 저장한다.
		int ReverseNum = logic.ChangePices(reversIndex, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard).Num();

		// aiBoardIndex의 가중치가 가장 높은 것을 찾는다.
		if (aiWeight <= aiBoard[reversIndex])
		{
			aiWeight = aiBoard[reversIndex]; // 가장 높은 가중치를 저장한다.

			if (maxRevers < ReverseNum) // 가중치가 높은데 많이 뒤집는 것이 있다면 저장해 놓는다.
			{
				// 가장 큰 놈의 위치를 저장한다.
				maxRevers = ReverseNum;
				bestPos = reversIndex;
				indexPos = reversIndex;
			}
			else // 없다면 가중치가 높은 자리의 값만 저장해 놓는다.
			{
				indexPos = reversIndex;
			}
		}
	}

	if (aiBoard[bestPos] == aiBoard[indexPos]) // 저장해 놓은 자리의 가중치가 같다면 가장 많이 뒤집는 자리로 바꾼다.
	{
		indexPos = bestPos;
	}
	// 뒤집을 위치를 저장한다.
	BoardInfoStruct.Index = indexPos;
	BoardInfoStruct.ReversePosArr = logic.ChangePices(indexPos, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	// 계산된 돌들을 보드에 기록한다.
	RecordPiece(BoardInfoStruct.Index, BoardInfoStruct.ReversePosArr);
	ReverseTurn(); // 턴 뒤집음

	// 점수를 세준다.
	for (int32 i = 0; i < GameInfoStruct.Size * GameInfoStruct.Size; i++)
	{
		whiteScore = othelloBoard[i] == WHITE_PIECE ? ++whiteScore : whiteScore;
		blackScore = othelloBoard[i] == BLACK_PIECE ? ++blackScore : blackScore;
	}
	BoardInfoStruct.whiteScore = whiteScore;
	BoardInfoStruct.blackScore = blackScore;

	// 플레이어가 착수 가능한 위치를 계산한다.
	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY) // 착수 가능한 곳이 없다면 ai가 다시 둔다.
	{
		ReverseTurn();
		BoardInfoStruct.PlacementPosArr.Empty();
		BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
		// 턴을 뒤집었는데도 둘 수 있는 곳이 없다면 게임을 종료한다.
		if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
		{
			// 뒤집어야 할 것들이 남았다면 턴을 원래대로 바꾸고 돌을 뒤집는다.
			if (BoardInfoStruct.ReversePosArr.Num() != EMPTY)
			{
				ReverseTurn();
				// 뒤집어야 하므로 착수 이벤트 브로드캐스트
				OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
			}

			// 게임오버 브로드캐스트
			OnTurnUpdated.Broadcast(TEXT("GameOver"), BoardInfoStruct);
		}
		// 아니라면 ai가 다시 둔다.
		else
		{
			OnTurnUpdated.Broadcast(TEXT("TurnOut"), BoardInfoStruct);
			AiPlacement(); // 재귀를 통해서 ai가 다시 둔다.
		}
	}
	else // 착수 가능하다면 돌을 둔다
	{
		// BoardInfoStruct를 브로드캐스트 하여 해당 이벤트를 구독하는 다른 곳에서 사용가능하게 만든다.
		OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
	}
	GetWorld()->GetTimerManager().ClearTimer(AiTimer); // 메모리 누수 방지를 위한 타이머 해제
}

void ASinglePlayGameMode::RecordPiece(int32 index, TArray<int32> ReversePos)
{
	int8 othelloPiece = 0;

	// 현재 턴이 검정돌 턴이면 보드에 검정돌을 기록 아니라면 하얀돌을 기록한다.
	othelloPiece = BoardInfoStruct.Turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;
	othelloBoard[index] = othelloPiece;

	for (int i = 0; i < ReversePos.Num(); i++)
	{
		int32 arrindex = ReversePos[i];
		othelloBoard[arrindex] = othelloPiece;
	}

}

void ASinglePlayGameMode::TimeOut()
{
	OthelloCalculation logic;
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

	if (BoardInfoStruct.Turn == WHITE_TURN)
	{
		AiPlacement();
	}
}

void ASinglePlayGameMode::ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ASinglePlayGameMode::SetAi()
{
	OthelloCalculation logic;

	const int32 BOARD_SIZE = GameInfoStruct.Size;
	const int32 MAX_SIZE = BOARD_SIZE - 1; // 보드의 가로 크기
	const int32 AVOID_POS = MAX_SIZE - 1; // 맨 끝자리의 바로 아래줄들은 맨 끝자리를 줘버리기 때문에 우선순위를 낮춤
	const int32 GOOD_POS = AVOID_POS - 1; // 피해야 하는 위치의 다음줄은 상대에게 안좋은 자리를 강요하기 때문에 우선순위를 높임

	// ai 가중치를 저장할 보드 정의
	aiBoard.Empty();
	aiBoard.Init(11, BOARD_SIZE * BOARD_SIZE);

	// 가중치 설정
	// 우선순위가 높은 위치로 테두리를 설정한다
	int32 aiIndex = 0;
	for (int32 i = 0; i < 2; i++)
	{
		// 가로
		for (int32 j = 1; j < MAX_SIZE; j++)
		{
			aiBoard[logic.Board_index(j, aiIndex, BOARD_SIZE)] = GOOD_POSITION;
		}
		// 세로
		for (int32 k = 1; k < MAX_SIZE; k++)
		{
			aiBoard[logic.Board_index(aiIndex, k, BOARD_SIZE)] = GOOD_POSITION;
		}
		aiIndex = MAX_SIZE;
	}

	// 우선순위가 낮은 위치 테두리 바로 아래쪽 여기에 두면 테두리를 뺏긴다.
	aiIndex = 1;
	for (int32 i = 0; i < 2; i++)
	{
		// 가로
		for (int32 j = 2; j < AVOID_POS; j++)
		{
			aiBoard[logic.Board_index(j, aiIndex, BOARD_SIZE)] = NOT_GOOD_POSITION;
		}
		// 세로
		for (int32 k = 2; k < AVOID_POS; k++)
		{
			aiBoard[logic.Board_index(aiIndex, k, BOARD_SIZE)] = NOT_GOOD_POSITION;
		}
		aiIndex = AVOID_POS;
	}

	// 무조건 두어야 하는 위치 대각선 맨 끝을 의미한다.
	aiBoard[logic.Board_index(0, 0, BOARD_SIZE)] = BEST_POSITION;
	aiBoard[logic.Board_index(MAX_SIZE, 0, BOARD_SIZE)] = BEST_POSITION;
	aiBoard[logic.Board_index(0, MAX_SIZE, BOARD_SIZE)] = BEST_POSITION;
	aiBoard[logic.Board_index(MAX_SIZE, MAX_SIZE, BOARD_SIZE)] = BEST_POSITION;

	// 특히나 우선순위가 높은 위치 무조건 피해야 하는 위치의 바로 다음칸이다.

	// 왼쪽 상단 5개
	for (int x = 0; x < 2; x++)
	{
		aiBoard[logic.Board_index(x, 2, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = 0; y < 3; y++)
	{
		aiBoard[logic.Board_index(2, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// 왼쪽 하단 5개
	for (int x = 0; x < 2; x++)
	{
		aiBoard[logic.Board_index(x, GOOD_POS, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = GOOD_POS; y < MAX_SIZE; y++)
	{
		aiBoard[logic.Board_index(2, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// 오른쪽 상단 5개
	for (int x = GOOD_POS; x <= MAX_SIZE; x++)
	{
		aiBoard[logic.Board_index(x, 2, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = 0; y < 2; y++)
	{
		aiBoard[logic.Board_index(GOOD_POS, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// 오른쪽 하단 5개
	for (int x = GOOD_POS; x <= MAX_SIZE; x++)
	{
		aiBoard[logic.Board_index(x, GOOD_POS, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = AVOID_POS; y <= MAX_SIZE; y++)
	{
		aiBoard[logic.Board_index(GOOD_POS, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// 무조건 피해야 하는 위치 대각선 끝을 기준으로 징검다리 역할을 할 부분을 차단한다.	
	// 왼쪽 상단 3개
	aiBoard[logic.Board_index(0, 1, BOARD_SIZE)] = BAD_POSITION;
	for (int y = 0; y < 2; y++)
	{
		aiBoard[logic.Board_index(1, y, BOARD_SIZE)] = BAD_POSITION;
	}

	// 오른쪽 상단 3개
	aiBoard[logic.Board_index(MAX_SIZE, 1, BOARD_SIZE)] = BAD_POSITION;
	for (int y = 0; y < 2; y++)
	{
		aiBoard[logic.Board_index(AVOID_POS, y, BOARD_SIZE)] = BAD_POSITION;
	}

	// 왼쪽 하단 3개
	aiBoard[logic.Board_index(1, MAX_SIZE, BOARD_SIZE)] = BAD_POSITION;
	for (int x = 0; x < 2; x++)
	{
		aiBoard[logic.Board_index(x, AVOID_POS, BOARD_SIZE)] = BAD_POSITION;
	}

	// 오른쪽 하단 3개
	aiBoard[logic.Board_index(AVOID_POS, AVOID_POS, BOARD_SIZE)] = BAD_POSITION;
	aiBoard[logic.Board_index(AVOID_POS, MAX_SIZE, BOARD_SIZE)] = BAD_POSITION;
	aiBoard[logic.Board_index(MAX_SIZE, AVOID_POS, BOARD_SIZE)] = BAD_POSITION;

	int y = BOARD_SIZE / 2;
	int x = (BOARD_SIZE / 2 - 1);

	// 보드 맨 처음 세팅
	int index = logic.Board_index(x, y, BOARD_SIZE);
	aiBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y - 1, BOARD_SIZE);
	aiBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y, BOARD_SIZE);
	aiBoard[index] = WHITE_PIECE;
	index = logic.Board_index(x, y - 1, BOARD_SIZE);
	aiBoard[index] = WHITE_PIECE;
}


