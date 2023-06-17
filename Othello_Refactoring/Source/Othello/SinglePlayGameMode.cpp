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
	GameInfoStruct.Size = size; // ���߿� ������ �ٽ� �����ϰ� �� ��츦 ���� �ʱ�ȭ

	othelloBoard.Empty();
	othelloBoard.Init(EMPTY, size * size);
	SetAi();

	int y = GameInfoStruct.Size / 2;
	int x = (GameInfoStruct.Size / 2 - 1);

	// ���� �� ó�� ����
	int index = logic.Board_index(x, y, size);
	othelloBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y - 1, size);
	othelloBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y, size);
	othelloBoard[index] = WHITE_PIECE;
	index = logic.Board_index(x, y - 1, size);
	othelloBoard[index] = WHITE_PIECE;

	OnGameInfoUpdated.Broadcast(TEXT("GameInfo"), GameInfoStruct); // �̺�Ʈ ��ε�ĳ��Ʈ �ش� �̺�Ʈ�� �����ϴ� Ŭ������ �����͸� ������

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

	// x, y��ǥ�� 1���� �迭�� ��ȯ�Ѵ�.
	BoardInfoStruct.Index = logic.Board_index(pX, pY, GameInfoStruct.Size);

	// ������� �ϴ� ��ġ�� ����Ѵ�.
	BoardInfoStruct.ReversePosArr = logic.ChangePices(pX, pY, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);

	// ���� ������ ���忡 ����Ѵ�.
	RecordPiece(BoardInfoStruct.Index, BoardInfoStruct.ReversePosArr);
	ReverseTurn(); // �� ������

	// ������ ���ش�.
	for (int32 i = 0; i < GameInfoStruct.Size * GameInfoStruct.Size; i++)
	{
		whiteScore = othelloBoard[i] == WHITE_PIECE ? ++whiteScore : whiteScore;
		blackScore = othelloBoard[i] == BLACK_PIECE ? ++blackScore : blackScore;
	}
	BoardInfoStruct.whiteScore = whiteScore;
	BoardInfoStruct.blackScore = blackScore;

	// ���� ������ ��ġ�� ����Ѵ�.
	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);

	// �� �̻� �� �� �ִ� ���� ���ٸ� ���� ����� �ٽ� ����Ѵ�.
	if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
	{
		ReverseTurn();
		BoardInfoStruct.PlacementPosArr.Empty();
		BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
		// ���� �������µ��� �� �� �ִ� ���� ���ٸ� ������ �����Ѵ�.
		if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
		{
			// ������� �� �͵��� ���Ҵٸ� ���� ������� �ٲٰ� ���� �����´�.
			if (BoardInfoStruct.ReversePosArr.Num() != EMPTY)
			{
				ReverseTurn();
				// ������� �ϹǷ� ���� �̺�Ʈ ��ε�ĳ��Ʈ
				OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
			}

			// ���ӿ��� ��ε�ĳ��Ʈ
			OnTurnUpdated.Broadcast(TEXT("GameOver"), BoardInfoStruct);
		}
		// �ƴ϶�� �ϸ� �����´�.
		else
		{
			OnTurnUpdated.Broadcast(TEXT("TurnOut"), BoardInfoStruct);
			GetWorld()->GetTimerManager().SetTimer(AiTimer, this, &ASinglePlayGameMode::AiPlacement, 2.f, false, 1.0);
		}
	}
	else
	{
		// BoardInfoStruct�� ��ε�ĳ��Ʈ �Ͽ� �ش� �̺�Ʈ�� �����ϴ� �ٸ� ������ ��밡���ϰ� �����.
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
		int reversIndex = BoardInfoStruct.PlacementPosArr[i]; // ������ �� �ִ� ��ġ�� �����Ѵ�.
		// ������ �� �ִ� ������ �����Ѵ�.
		int ReverseNum = logic.ChangePices(reversIndex, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard).Num();

		// aiBoardIndex�� ����ġ�� ���� ���� ���� ã�´�.
		if (aiWeight <= aiBoard[reversIndex])
		{
			aiWeight = aiBoard[reversIndex]; // ���� ���� ����ġ�� �����Ѵ�.

			if (maxRevers < ReverseNum) // ����ġ�� ������ ���� ������ ���� �ִٸ� ������ ���´�.
			{
				// ���� ū ���� ��ġ�� �����Ѵ�.
				maxRevers = ReverseNum;
				bestPos = reversIndex;
				indexPos = reversIndex;
			}
			else // ���ٸ� ����ġ�� ���� �ڸ��� ���� ������ ���´�.
			{
				indexPos = reversIndex;
			}
		}
	}

	if (aiBoard[bestPos] == aiBoard[indexPos]) // ������ ���� �ڸ��� ����ġ�� ���ٸ� ���� ���� ������ �ڸ��� �ٲ۴�.
	{
		indexPos = bestPos;
	}
	// ������ ��ġ�� �����Ѵ�.
	BoardInfoStruct.Index = indexPos;
	BoardInfoStruct.ReversePosArr = logic.ChangePices(indexPos, BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	// ���� ������ ���忡 ����Ѵ�.
	RecordPiece(BoardInfoStruct.Index, BoardInfoStruct.ReversePosArr);
	ReverseTurn(); // �� ������

	// ������ ���ش�.
	for (int32 i = 0; i < GameInfoStruct.Size * GameInfoStruct.Size; i++)
	{
		whiteScore = othelloBoard[i] == WHITE_PIECE ? ++whiteScore : whiteScore;
		blackScore = othelloBoard[i] == BLACK_PIECE ? ++blackScore : blackScore;
	}
	BoardInfoStruct.whiteScore = whiteScore;
	BoardInfoStruct.blackScore = blackScore;

	// �÷��̾ ���� ������ ��ġ�� ����Ѵ�.
	BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
	if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY) // ���� ������ ���� ���ٸ� ai�� �ٽ� �д�.
	{
		ReverseTurn();
		BoardInfoStruct.PlacementPosArr.Empty();
		BoardInfoStruct.PlacementPosArr = logic.IsPutOthello(BoardInfoStruct.Turn, GameInfoStruct.Size, othelloBoard);
		// ���� �������µ��� �� �� �ִ� ���� ���ٸ� ������ �����Ѵ�.
		if (BoardInfoStruct.PlacementPosArr.Num() == EMPTY)
		{
			// ������� �� �͵��� ���Ҵٸ� ���� ������� �ٲٰ� ���� �����´�.
			if (BoardInfoStruct.ReversePosArr.Num() != EMPTY)
			{
				ReverseTurn();
				// ������� �ϹǷ� ���� �̺�Ʈ ��ε�ĳ��Ʈ
				OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
			}

			// ���ӿ��� ��ε�ĳ��Ʈ
			OnTurnUpdated.Broadcast(TEXT("GameOver"), BoardInfoStruct);
		}
		// �ƴ϶�� ai�� �ٽ� �д�.
		else
		{
			OnTurnUpdated.Broadcast(TEXT("TurnOut"), BoardInfoStruct);
			AiPlacement(); // ��͸� ���ؼ� ai�� �ٽ� �д�.
		}
	}
	else // ���� �����ϴٸ� ���� �д�
	{
		// BoardInfoStruct�� ��ε�ĳ��Ʈ �Ͽ� �ش� �̺�Ʈ�� �����ϴ� �ٸ� ������ ��밡���ϰ� �����.
		OnTurnUpdated.Broadcast(TEXT("Placement"), BoardInfoStruct);
	}
	GetWorld()->GetTimerManager().ClearTimer(AiTimer); // �޸� ���� ������ ���� Ÿ�̸� ����
}

void ASinglePlayGameMode::RecordPiece(int32 index, TArray<int32> ReversePos)
{
	int8 othelloPiece = 0;

	// ���� ���� ������ ���̸� ���忡 �������� ��� �ƴ϶�� �Ͼᵹ�� ����Ѵ�.
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
		// BoardInfoStruct�� ��ε�ĳ��Ʈ �Ͽ� �ش� �̺�Ʈ�� �����ϴ� �ٸ� ������ ��밡���ϰ� �����.
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
	const int32 MAX_SIZE = BOARD_SIZE - 1; // ������ ���� ũ��
	const int32 AVOID_POS = MAX_SIZE - 1; // �� ���ڸ��� �ٷ� �Ʒ��ٵ��� �� ���ڸ��� ������� ������ �켱������ ����
	const int32 GOOD_POS = AVOID_POS - 1; // ���ؾ� �ϴ� ��ġ�� �������� ��뿡�� ������ �ڸ��� �����ϱ� ������ �켱������ ����

	// ai ����ġ�� ������ ���� ����
	aiBoard.Empty();
	aiBoard.Init(11, BOARD_SIZE * BOARD_SIZE);

	// ����ġ ����
	// �켱������ ���� ��ġ�� �׵θ��� �����Ѵ�
	int32 aiIndex = 0;
	for (int32 i = 0; i < 2; i++)
	{
		// ����
		for (int32 j = 1; j < MAX_SIZE; j++)
		{
			aiBoard[logic.Board_index(j, aiIndex, BOARD_SIZE)] = GOOD_POSITION;
		}
		// ����
		for (int32 k = 1; k < MAX_SIZE; k++)
		{
			aiBoard[logic.Board_index(aiIndex, k, BOARD_SIZE)] = GOOD_POSITION;
		}
		aiIndex = MAX_SIZE;
	}

	// �켱������ ���� ��ġ �׵θ� �ٷ� �Ʒ��� ���⿡ �θ� �׵θ��� �����.
	aiIndex = 1;
	for (int32 i = 0; i < 2; i++)
	{
		// ����
		for (int32 j = 2; j < AVOID_POS; j++)
		{
			aiBoard[logic.Board_index(j, aiIndex, BOARD_SIZE)] = NOT_GOOD_POSITION;
		}
		// ����
		for (int32 k = 2; k < AVOID_POS; k++)
		{
			aiBoard[logic.Board_index(aiIndex, k, BOARD_SIZE)] = NOT_GOOD_POSITION;
		}
		aiIndex = AVOID_POS;
	}

	// ������ �ξ�� �ϴ� ��ġ �밢�� �� ���� �ǹ��Ѵ�.
	aiBoard[logic.Board_index(0, 0, BOARD_SIZE)] = BEST_POSITION;
	aiBoard[logic.Board_index(MAX_SIZE, 0, BOARD_SIZE)] = BEST_POSITION;
	aiBoard[logic.Board_index(0, MAX_SIZE, BOARD_SIZE)] = BEST_POSITION;
	aiBoard[logic.Board_index(MAX_SIZE, MAX_SIZE, BOARD_SIZE)] = BEST_POSITION;

	// Ư���� �켱������ ���� ��ġ ������ ���ؾ� �ϴ� ��ġ�� �ٷ� ����ĭ�̴�.

	// ���� ��� 5��
	for (int x = 0; x < 2; x++)
	{
		aiBoard[logic.Board_index(x, 2, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = 0; y < 3; y++)
	{
		aiBoard[logic.Board_index(2, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// ���� �ϴ� 5��
	for (int x = 0; x < 2; x++)
	{
		aiBoard[logic.Board_index(x, GOOD_POS, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = GOOD_POS; y < MAX_SIZE; y++)
	{
		aiBoard[logic.Board_index(2, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// ������ ��� 5��
	for (int x = GOOD_POS; x <= MAX_SIZE; x++)
	{
		aiBoard[logic.Board_index(x, 2, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = 0; y < 2; y++)
	{
		aiBoard[logic.Board_index(GOOD_POS, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// ������ �ϴ� 5��
	for (int x = GOOD_POS; x <= MAX_SIZE; x++)
	{
		aiBoard[logic.Board_index(x, GOOD_POS, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}
	for (int y = AVOID_POS; y <= MAX_SIZE; y++)
	{
		aiBoard[logic.Board_index(GOOD_POS, y, BOARD_SIZE)] = VERY_GOOD_POSITION;
	}

	// ������ ���ؾ� �ϴ� ��ġ �밢�� ���� �������� ¡�˴ٸ� ������ �� �κ��� �����Ѵ�.	
	// ���� ��� 3��
	aiBoard[logic.Board_index(0, 1, BOARD_SIZE)] = BAD_POSITION;
	for (int y = 0; y < 2; y++)
	{
		aiBoard[logic.Board_index(1, y, BOARD_SIZE)] = BAD_POSITION;
	}

	// ������ ��� 3��
	aiBoard[logic.Board_index(MAX_SIZE, 1, BOARD_SIZE)] = BAD_POSITION;
	for (int y = 0; y < 2; y++)
	{
		aiBoard[logic.Board_index(AVOID_POS, y, BOARD_SIZE)] = BAD_POSITION;
	}

	// ���� �ϴ� 3��
	aiBoard[logic.Board_index(1, MAX_SIZE, BOARD_SIZE)] = BAD_POSITION;
	for (int x = 0; x < 2; x++)
	{
		aiBoard[logic.Board_index(x, AVOID_POS, BOARD_SIZE)] = BAD_POSITION;
	}

	// ������ �ϴ� 3��
	aiBoard[logic.Board_index(AVOID_POS, AVOID_POS, BOARD_SIZE)] = BAD_POSITION;
	aiBoard[logic.Board_index(AVOID_POS, MAX_SIZE, BOARD_SIZE)] = BAD_POSITION;
	aiBoard[logic.Board_index(MAX_SIZE, AVOID_POS, BOARD_SIZE)] = BAD_POSITION;

	int y = BOARD_SIZE / 2;
	int x = (BOARD_SIZE / 2 - 1);

	// ���� �� ó�� ����
	int index = logic.Board_index(x, y, BOARD_SIZE);
	aiBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y - 1, BOARD_SIZE);
	aiBoard[index] = BLACK_PIECE;
	index = logic.Board_index(x + 1, y, BOARD_SIZE);
	aiBoard[index] = WHITE_PIECE;
	index = logic.Board_index(x, y - 1, BOARD_SIZE);
	aiBoard[index] = WHITE_PIECE;
}


