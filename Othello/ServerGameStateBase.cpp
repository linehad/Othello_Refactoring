// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameStateBase.h"
#include "GameBoard.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

#include "Net/UnrealNetwork.h"
void AServerGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AServerGameStateBase, limitTime);
	DOREPLIFETIME(AServerGameStateBase, boardSize);
}

void AServerGameStateBase::SetData(int32 time, int32 size)
{
	limitTime = time;
	boardSize = size;
	arrOthelloButton.Empty();
	//arrOthelloButton.Init(EMPTY, size);
}
