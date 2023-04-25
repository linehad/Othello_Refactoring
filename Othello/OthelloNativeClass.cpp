// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloNativeClass.h"
#include "OthelloPices_UserWidget.h"

void OthelloNativeClass::ChangePices(int x, int y, int8 piceColor, int boardSize, TArray <UOthelloPices_UserWidget*>arrOthelloButton)
{

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // 자신의 색을 만나면 true 그 사이에 있는 것들을 뒤집는다.

		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize && arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPice() != 0)
		{
			if (arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPice() == piceColor)
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
			// 실제로 뒤집는다.
			arrOthelloButton[Board_index(nx, ny, boardSize)]->ReversePice();

			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
}

// 둘수 있는 위치를 표시해주는 함수
bool OthelloNativeClass::PutOthello(int x, int y, int8 piceColor, int boardSize, TArray <class UOthelloPices_UserWidget*>arrOthelloButton)
{

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize
			&& arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPice() != 0)
		{

			// 바로 다음돌이 내 색이랑 같을 경우
			if (piceColor == arrOthelloButton[Board_index(x + arr_dX[i], y + arr_dY[i], boardSize)]->GetPice())
			{
				break;
			}
			else if (arrOthelloButton[Board_index(nx, ny, boardSize)]->GetPice() == piceColor)
			{
				return true;
			}
			nx += arr_dX[i];
			ny += arr_dY[i];
		}
	}
	return false;
}