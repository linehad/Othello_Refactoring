#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInfoStruct.h"
#include "GameBoard.generated.h"

/**
 *
 */
UCLASS()
class OTHELLO_API UGameBoard : public UUserWidget
{
	GENERATED_BODY()
private:
	FGameInfoStruct GameInfoStruct;
	FBoardInfoStruct LastBoardInfo;

	class AServerGameStateBase* GameState;

public:
	virtual void NativeConstruct() override;
	UPROPERTY()
		TArray <class UOthelloPices_UserWidget*>arrOthelloButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf <class UOthelloPices_UserWidget>OthelloButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UUniformGridPanel* Board_UniformGridPanel = nullptr;

	// 흑돌, 백돌 이미지
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* WhiteImage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* BlackImage = nullptr;


	// 게임의 흐름을 담당하는 함수들
public:
	// 처음 시작할 때 호출할 함수
	void StartSet();

	// 1차원 배열을 2차월 배열 처럼 쓰기 위한 함수
	UFUNCTION()
		inline int16 Board_index(int x, int y) { return (GameInfoStruct.Size * y) + x; }

	void PossiblePiece(const FBoardInfoStruct& Data);

	// 착수 기능
	UFUNCTION()
	void Placement(FName EventName, const FBoardInfoStruct& Data);
	void Placement(int32 index, bool turn);
	void Placement(int8 x, int8 y, bool turn);

};