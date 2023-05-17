// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameStateBase.h"
#include "GameBoard.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

void AServerGameStateBase::SetData(int32 time, int32 size)
{
	limitTime = time;
	boardSize = size;
	arrOthelloButton.Empty();
	//arrOthelloButton.Init(EMPTY, size);
}
