// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameStateBase.h"
#include "OthelloGameModeBase.h"
#include "GameBoard.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

#include "Net/UnrealNetwork.h"

AServerGameStateBase::AServerGameStateBase()
{
	bReplicates = true;
}

void AServerGameStateBase::BindEvent()
{
	AOthelloGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AOthelloGameModeBase>();
	if (GameMode) //클라에서는 접근이 불가능하기 때문에 서버에서만 실행이 된다.
	{
		GameMode->OnGameInfoUpdated.AddDynamic(this, &AServerGameStateBase::Server_GameInfoStructUpdated);
		GameMode->OnTurnUpdated.AddDynamic(this, &AServerGameStateBase::Server_OnTurnUpdated);
	}
}

void AServerGameStateBase::Server_GameInfoStructUpdated(FName PropertyName, const FGameInfoStruct& Data)
{
	if (HasAuthority())
	{

		if (PropertyName == TEXT("GameInfo"))
		{
			UE_LOG(LogTemp, Log, TEXT("server "));
			Multicast_GameInfoStruct(Data);
		}
	}
}

void AServerGameStateBase::Multicast_GameInfoStruct_Implementation(const FGameInfoStruct& Data)
{
	GameInfoStruct = Data;
}

void AServerGameStateBase::Server_OnTurnUpdated(const FBoardInfoStruct& BoardInfo)
{
	if (HasAuthority())
	{
		Multicast_BoardInfo(BoardInfo);
	}
}

void AServerGameStateBase::Multicast_BoardInfo_Implementation(const FBoardInfoStruct& BoardInfo)
{
	BoardInfoStruct = BoardInfo;
}