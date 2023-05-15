// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInfoStruct.generated.h"

// �����Ϸ��� C�� �д� ��ó�� ������ ����ü ���漱���� �ϰų� ����ü�� ������ �ö�;� ��
USTRUCT()
struct FGameInfoStruct
{
	GENERATED_BODY()

public:
	int32 Size = 0;
	int32 Time = 0;
	int8 GameTurn = 0;
};

class OTHELLO_API GameInfoStruct
{
public:

};
