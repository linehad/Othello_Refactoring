// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameStateBase.h"
#include "SinglePlayGameMode.h"
#include "OthelloGameModeBase.h"
#include "GameBoard.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

#include "Engine.h"
#include "Net/UnrealNetwork.h"

AServerGameStateBase::AServerGameStateBase()
{
	bReplicates = true;
}

void AServerGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AServerGameStateBase, BoardInfoStruct);
}

void AServerGameStateBase::BindEvent()
{
	AOthelloGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AOthelloGameModeBase>();
	ASinglePlayGameMode* SingleGameMod = GetWorld()->GetAuthGameMode<ASinglePlayGameMode>();

	if (GameMode) //클라에서는 접근이 불가능하기 때문에 서버에서만 실행이 된다.
	{
		GameMode->OnGameInfoUpdated.AddDynamic(this, &AServerGameStateBase::Server_GameInfoStructUpdated);
		GameMode->OnTurnUpdated.AddDynamic(this, &AServerGameStateBase::Server_OnTurnUpdated);
	}
	else if (SingleGameMod) //싱글 전용
	{
		SingleGameMod->OnGameInfoUpdated.AddDynamic(this, &AServerGameStateBase::Server_GameInfoStructUpdated);
		SingleGameMod->OnTurnUpdated.AddDynamic(this, &AServerGameStateBase::Server_OnTurnUpdated);
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

void AServerGameStateBase::Multicast_BoardInfoStruct_Implementation(FName EventName, const FBoardInfoStruct& BoardInfo)
{
	OnPlacementEvent.Broadcast(EventName, BoardInfo);
}

void AServerGameStateBase::Server_OnTurnUpdated(FName EventName, const FBoardInfoStruct& BoardInfo)
{
	if (HasAuthority())
	{
		BoardInfoStruct = BoardInfo;
		Multicast_BoardInfoStruct(EventName, BoardInfo);
	}
}