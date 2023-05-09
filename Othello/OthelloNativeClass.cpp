// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloNativeClass.h"
#include "OthelloPices_UserWidget.h"

// �̰͵� ������ ���길 �ؼ� ������� �Ǵ� �ֵ��� ��ġ�� �Ѱ���ߵ�
void OthelloNativeClass::ChangePices(int x, int y, int8 turn, TArray <int32>arrOthelloButton)
{
	int32 boardSize = arrOthelloButton.Num();
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // �ڽ��� ���� ������ true �� ���̿� �ִ� �͵��� �����´�.

		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize && arrOthelloButton[Board_index(nx, ny, boardSize)] != 0)
		{
			if (arrOthelloButton[Board_index(nx, ny, boardSize)] == piece)
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
			arrOthelloButton[Board_index(nx, ny, boardSize)]->ReversePiece(); // �� �κ��� ������� �� ��ġ�� �����ؼ� ��������� �Ѵ�

			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
}

// �Ű� ���� �ٲ㼭 �ٽ� ���� �ؾߵ� index ���� ���� ���� ���ϰ� ������
// �Ѽ� �ִ� ��ġ�� ǥ�����ִ� �Լ�
bool OthelloNativeClass::IsPutOthello(int x, int y, int8 turn, TArray <int32>arrOthelloButton)
{
	int32 boardSize = arrOthelloButton.Num();
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize
			&& arrOthelloButton[Board_index(nx, ny, boardSize)] != 0) // ���� �ȿ� ���� ������� �� �ݺ�
		{

			// �ٷ� �������� �� ���̶� ���� ���
			if (piece == arrOthelloButton[Board_index(x + arr_dX[i], y + arr_dY[i], boardSize)])
			{
				break;
			}
			else if (arrOthelloButton[Board_index(nx, ny, boardSize)] == piece)
			{
				return true;
			}
			nx += arr_dX[i];
			ny += arr_dY[i];
		}
	}
	return false;
}