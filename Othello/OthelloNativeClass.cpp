// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloNativeClass.h"
#include "OthelloPices_UserWidget.h"

// �̰͵� ������ ���길 �ؼ� ������� �Ǵ� �ֵ��� ��ġ�� �Ѱ���ߵ�
void OthelloNativeClass::ChangePices(int x, int y, int8 piceColor, int boardSize, TArray <UOthelloPices_UserWidget*>arrOthelloButton)
{

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // �ڽ��� ���� ������ true �� ���̿� �ִ� �͵��� �����´�.

		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize && arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPiece() != 0)
		{
			if (arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPiece() == piceColor)
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
			arrOthelloButton[Board_index(nx, ny, boardSize)]->ReversePiece();

			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
}

// �Ű� ���� �ٲ㼭 �ٽ� ���� �ؾߵ� index ���� ���� ���� ���ϰ� ������
// �Ѽ� �ִ� ��ġ�� ǥ�����ִ� �Լ�
bool OthelloNativeClass::PutOthello(int x, int y, int8 piceColor, int boardSize, TArray <class UOthelloPices_UserWidget*>arrOthelloButton)
{

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize
			&& arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPiece() != 0)
		{

			// �ٷ� �������� �� ���̶� ���� ���
			if (piceColor == arrOthelloButton[Board_index(x + arr_dX[i], y + arr_dY[i], boardSize)]->GetPiece())
			{
				break;
			}
			else if (arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPiece() == piceColor)
			{
				return true;
			}
			nx += arr_dX[i];
			ny += arr_dY[i];
		}
	}
	return false;
}