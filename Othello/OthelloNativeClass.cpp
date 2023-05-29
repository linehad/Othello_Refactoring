#include "OthelloNativeClass.h"
#include "OthelloPices_UserWidget.h"

// �̰͵� ������ ���길 �ؼ� ������� �Ǵ� �ֵ��� ��ġ�� �Ѱ���ߵ�
TArray <int32> OthelloNativeClass::ChangePices(int x, int y, int8 turn, int16 row, TArray <int32>board)
{
	int32 boardSize = row;
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;
	TArray <int32>reverseIndex;
	reverseIndex.Empty();

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // �ڽ��� ���� ������ true �� ���̿� �ִ� �͵��� �����´�.

		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize && board[Board_index(nx, ny, boardSize)] != 0)
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
			// ������ �����´�.
			//arrOthelloButton[Board_index(nx, ny, boardSize)]->ReversePiece(); // �� �κ��� ������� �� ��ġ�� �����ؼ� ��������� �Ѵ�
			reverseIndex.Add(Board_index(nx, ny, boardSize));
			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
	return reverseIndex;
}

// �Ѽ� �ִ� ��ġ�� ǥ�����ִ� �Լ�
bool OthelloNativeClass::IsPutOthello(int x, int y, int8 turn, int16 row, TArray <int32>board)
{
	int32 boardSize = row;
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize
			&& board[Board_index(nx, ny, boardSize)] != 0) // ���� �ȿ� ���� ������� �� �ݺ�
		{

			// �ٷ� �������� �� ���̶� ���� ���
			if (piece == board[Board_index(x + arr_dX[i], y + arr_dY[i], boardSize)])
			{
				break;
			}
			else if (board[Board_index(nx, ny, boardSize)] == piece)
			{
				return true;
			}
			nx += arr_dX[i];
			ny += arr_dY[i];
		}
	}
	return false;
}