// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInfoStruct.generated.h"

// 컴파일러가 C언어를 읽는 것처럼 동작함 구조체 전방선언을 하거나 구조체가 맨위로 올라와야 함
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
