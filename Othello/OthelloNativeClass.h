#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class OTHELLO_API OthelloNativeClass
{
private:
		TArray<int8> arr_dX = { -1, -1, -1, 0, 0 , 1, 1, 1 };
		TArray<int8> arr_dY = { -1, 0, 1, -1, 1 , -1, 0, 1 };
	const int DIRECTUIN = 8;
public:
	// 돌의 색을 뒤집는 함수
	TArray <int32> ChangePices(int x, int y, int8 turn, TArray <int32>arrOthelloButton);
	// 둘수 있는 위치를 표시하는 함수
		bool IsPutOthello(int x, int y, int8 turn, TArray <int32>arrOthelloButton);
	// 1차원 배열을 2차월 배열 처럼 쓰기 위한 함수
		inline int16 Board_index(int x, int y, int boardSize) { return (boardSize * y) + x; }
};