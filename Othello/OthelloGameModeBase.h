#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameInfoStruct.h"
#include "OthelloGameModeBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameInfoStructUpdated, FName, PropertyName, const FGameInfoStruct&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoardUpdatedEvent, const FBoardInfoStruct&, BoardInfoStruct);

UCLASS()
class OTHELLO_API AOthelloGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FGameInfoStruct GameInfoStruct;
	UPROPERTY()
		FBoardInfoStruct BoardInfoStruct;
	//UPROPERTY(BlueprintReadWrite, Category = "GameInfo")
	int16 changeCount = 0;
	const int16 ENTRY = 0;

		int16 beforeIndex = 0;
		bool bcheckTurn = false;
		int16 othelloArrIndex; // ���� ��ġ�� ������ ����

	UPROPERTY()
	TArray <int32> othelloBoard; // ���� ������ ���� ��Ȳ
public:
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void SetStartingWidget(TSubclassOf<UUserWidget> StartingWidget){ StartingWidgetClass = StartingWidget;	}

	// ���� ���� �Ǿ��� �� ���� �� �Լ�
	UFUNCTION(Server, Reliable)
		void SetOthelloNextturn();
	void SetOthelloNextturn_Implementation();

	// ó�� ���۽� ������ ������� ���� �ð��� �����ϴ� �Լ�
		void SetGameData(int size, int time);

	// ���� ���� �Ǿ��� �� ���� �� �Լ�
		void OthelloNextTurn(int arrIndex);

	// ������ ���� ������ �Լ�
		void ReverseTurn(); // �ð� �ʰ� �Ѿ�� ���� ������ ���� �Ѿ� ���� �� ��� ȣ��

	UFUNCTION()
		TArray<int8> OthelloChangeTurn(int32 pX, int32 py); // �����Ͽ� ���� ���� �÷��̾� ���� ���� ���� ���� ��ġ�� ǥ�����ִ� �Լ�

	void SetOthelloArrIndex(int16 index) { othelloArrIndex = index; }

	// �̺�Ʈ ��� ������
	UPROPERTY()
	FGameInfoStructUpdated OnGameInfoUpdated;

	UPROPERTY()
		FBoardUpdatedEvent OnTurnUpdated;
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BoardWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;
};