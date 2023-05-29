// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInfoStruct.generated.h"

USTRUCT(BlueprintType)
struct OTHELLO_API FGameInfoStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		int32 Size;

	UPROPERTY()
		int32 Time;

	FGameInfoStruct()
		: Size(0), Time(0)
	{
	}
};

USTRUCT(BlueprintType)
struct OTHELLO_API FBoardInfoStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		TArray <int8> BoardArr;
	UPROPERTY()
		bool Turn = true;
};