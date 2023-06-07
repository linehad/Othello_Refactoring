#include "OthelloNativeClass.h"
#include "OthelloPices_UserWidget.h"

TArray <int32> OthelloNativeClass::ChangePices(int x, int y, int8 turn, int16 size, TArray <int32>board)
{
	int32 boardSize = size;
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;
	TArray <int32>reverseIndex;
	reverseIndex.Empty();

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // �ڽ��� ���� ������ true �� ���̿� �ִ� �͵��� �����´�.

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
			reverseIndex.Add(Board_index(nx, ny, boardSize)); // ������� �� ��ġ�� �߰���
			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
	return reverseIndex;
}

// �Ѽ� �ִ� ��ġ�� ǥ�����ִ� �Լ�
TArray <int32> OthelloNativeClass::IsPutOthello(int8 turn, int16 size, TArray <int32>board)
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

				while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize && // �迭 �ٱ����� ������ �ʰ�
					board[Board_index(row, col, boardSize)] == EMPTY && // ���� ��ġ�� ��� �ְ�
					board[Board_index(nx, ny, boardSize)] != EMPTY) // Ž�� ��ġ�� ���� �ִٸ�
				{
					// �ٷ� �������� �� ���̶� ���� ���
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