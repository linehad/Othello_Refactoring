// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameInfoStruct.h"
#include "ServerGameStateBase.generated.h"

UCLASS()
class OTHELLO_API AServerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		FGameInfoStruct GameInfoStruct;

	UPROPERTY()
		FBoardInfoStruct BoardInfoStruct;
protected:

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_BoardInfo(const FBoardInfoStruct& BoardInfo);
	void Multicast_BoardInfo_Implementation(const FBoardInfoStruct& BoardInfo);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_GameInfoStruct(const FGameInfoStruct& Data);
	void Multicast_GameInfoStruct_Implementation(const FGameInfoStruct& Data);
public:
	AServerGameStateBase();
	UFUNCTION()
		void BindEvent();

	// 이벤트 바인딩되는 함수
	UFUNCTION()
		void Server_OnTurnUpdated(const FBoardInfoStruct& BoardInfo);

	UFUNCTION()
		void Server_GameInfoStructUpdated(FName PropertyName, const FGameInfoStruct& Data);

	const FGameInfoStruct& GetGameInfoStruct() const { return GameInfoStruct; }
	bool GetGameTurn() { return BoardInfoStruct.Turn; }
};
