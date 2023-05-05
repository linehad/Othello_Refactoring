// Fill out your copyright notice in the Description page of Project Settings.

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
	// ���� ���� ������ �Լ�
		void ChangePices(int x, int y, int8 turn, TArray <int32>arrOthelloButton);
	// �Ѽ� �ִ� ��ġ�� ǥ���ϴ� �Լ�
		bool IsPutOthello(int x, int y, int8 turn, TArray <int32>arrOthelloButton);
	// 1���� �迭�� 2���� �迭 ó�� ���� ���� �Լ�
		inline int16 Board_index(int x, int y, int boardSize) { return (boardSize * y) + x; }
};