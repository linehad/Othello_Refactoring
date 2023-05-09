// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloNativeClass.h"
#include "OthelloPices_UserWidget.h"

// 이것도 개문제 연산만 해서 뒤집어야 되는 애들의 위치만 넘겨줘야됨
void OthelloNativeClass::ChangePices(int x, int y, int8 turn, TArray <int32>arrOthelloButton)
{
	int32 boardSize = arrOthelloButton.Num();
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		bool flag = false; // 자신의 색을 만나면 true 그 사이에 있는 것들을 뒤집는다.

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
			// 실제로 뒤집는다.
			arrOthelloButton[Board_index(nx, ny, boardSize)]->ReversePiece(); // 이 부분을 뒤집어야 할 위치를 저장해서 리턴해줘야 한다

			nx -= arr_dX[i];
			ny -= arr_dY[i];
		}
	}
}

// 매개 변수 바꿔서 다시 설계 해야됨 index 따로 만들어서 보기 편하게 만들자
// 둘수 있는 위치를 표시해주는 함수
bool OthelloNativeClass::IsPutOthello(int x, int y, int8 turn, TArray <int32>arrOthelloButton)
{
	int32 boardSize = arrOthelloButton.Num();
	int8 piece = turn == BLACK_TURN ? BLACK_PIECE : WHITE_PIECE;

	for (int i = 0; i < DIRECTUIN; i++)
	{
		int nx = x + arr_dX[i];
		int ny = y + arr_dY[i];
		while (nx >= 0 && ny >= 0 && nx < boardSize && ny < boardSize
			&& arrOthelloButton[Board_index(nx, ny, boardSize)] != 0) // 보드 안에 돌이 들어있을 때 반복
		{

			// 바로 다음돌이 내 색이랑 같을 경우
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