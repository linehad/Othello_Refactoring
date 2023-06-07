// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameInfoStruct.h"
#include "ServerGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlacementEvent, FName, PropertyName, const FBoardInfoStruct&, BoardInfoStruct);

UCLASS()
class OTHELLO_API AServerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		FGameInfoStruct GameInfoStruct;

	UPROPERTY(Replicated)
		FBoardInfoStruct BoardInfoStruct;
protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_GameInfoStruct(const FGameInfoStruct& Data);
	void Multicast_GameInfoStruct_Implementation(const FGameInfoStruct& Data);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_BoardInfoStruct(FName PropertyName, const FBoardInfoStruct& BoardInfo);
	void Multicast_BoardInfoStruct_Implementation(FName PropertyName, const FBoardInfoStruct& BoardInfo);

public:
	AServerGameStateBase();

	// 착수 이벤트 프로퍼티
	UPROPERTY()
	FPlacementEvent OnPlacementEvent;

	UFUNCTION()
		void BindEvent();

	// 이벤트 바인딩되는 함수
	UFUNCTION()
		void Server_OnTurnUpdated(FName EventName, const FBoardInfoStruct& BoardInfo);

	UFUNCTION()
		void Server_GameInfoStructUpdated(FName EventName, const FGameInfoStruct& Data);

	const FGameInfoStruct& GetGameInfoStruct() const { return GameInfoStruct; }
	const FBoardInfoStruct& GetBoardInfoStruct() const { return BoardInfoStruct; }

	bool GetGameTurn() { return BoardInfoStruct.Turn; }
};
