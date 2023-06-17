// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloCalculation.h"
#include "OthelloPices_UserWidget.h"

TArray <int32> OthelloCalculation::ChangePices(int x, int y, int8 turn, int16 size, TArray <int32>board)
{
	int32 boardSize = size;
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;
	TArray <int32>reverseIndex;
	reverseIndex.Empty();

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // 자신의 색을 만나면 true 그 사이에 있는 것들을 뒤집는다.

		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize &&
			board[Board_index(nx, ny, boardSize)] != 0)
		{
			if (board[Board_index(nx, ny, boardSize)] == piece)
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
			reverseIndex.Add(Board_index(nx, ny, boardSize)); // 뒤집어야 할 위치를 추가함
			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
	return reverseIndex;
}

TArray<int32> OthelloCalculation::ChangePices(int index, int8 turn, int16 size, TArray<int32> board)
{
	TTuple<int32, int32> Pos = GetPos(index, size);
	return ChangePices(Pos.Key, Pos.Value, turn, size, board);
}

// 둘수 있는 위치를 표시해주는 함수
TArray <int32> OthelloCalculation::IsPutOthello(int8 turn, int16 size, TArray <int32>board)
{
	int32 boardSize = size;
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;
	TArray <int32> MoveIndex;
	MoveIndex.Empty();

	for (int col = 0; col < boardSize; col++)
	{
		for (int row = 0; row < boardSize; row++)
		{
			for (int k = 0; k < DIRECTUIN; k++)
			{
				int nx = row + arr_dX[k];
				int ny = col + arr_dY[k];

				while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize && // 배열 바깥으로 나가지 않고
					board[Board_index(row, col, boardSize)] == EMPTY && // 현재 위치가 비어 있고
					board[Board_index(nx, ny, boardSize)] != EMPTY) // 탐색 위치에 돌이 있다면
				{
					// 바로 다음돌이 내 색이랑 같을 경우
					if (piece == board[Board_index(row + arr_dX[k], col + arr_dY[k], boardSize)])
					{
						break;
					}
					else if (board[Board_index(nx, ny, boardSize)] == piece)
					{
						MoveIndex.Add(Board_index(row, col, boardSize));
						break;
					}
					nx += arr_dX[k];
					ny += arr_dY[k];
				}

			}
		}
	}
	return MoveIndex;
}

TTuple<int32, int32> OthelloCalculation::GetPos(int32 index, int32 row)
{
	TArray<int32> IndexArr;
	IndexArr.Init(-1, row * row); // 보드의 전체 크기만큼 -1값을 넣어 초기화

	int y = 0;
	// y값을 추론하기 위한 배열 정의
	for (int i = 0; i < IndexArr.Num(); i++)
	{
		if (i != 0 && i % row == 0) // 배열에 y 값만 채워 넣어 준다. 0행에는 0 1행에는 1... 이런식으로 다 채워줌
		{
			y++;
		}
		IndexArr[i] = y;
	}
	y = IndexArr[index];

	int x = -1;
	// x값 추론
	for (int i = 0; i < row; i++)
	{
		if (Board_index(i, y, row) == index) // y값은 찾았으니 대조해 본다.
		{
			x = i;
			break;
		}
	}

	return  MakeTuple(x, y);
}
