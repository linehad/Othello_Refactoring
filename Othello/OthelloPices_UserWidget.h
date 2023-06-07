#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OthelloPices_UserWidget.generated.h"

enum GameCurrentStatus
{
	SERVER_PLAY,
	SET_START,
	CLIENT_PLAY,
};

enum PieceColor
{
	EMPTY = 0,
	BLACK_PIECE = 1,
	WHITE_PIECE = 2,
};

enum Turn
{
	BLACK_TURN,
	WHITE_TURN,
};

UCLASS()
class OTHELLO_API UOthelloPices_UserWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	class AServerGameStateBase* GameStateBase = nullptr;

	// false = 흑돌, true = 백돌
	// 0 = 빈곳, 1 = 흑돌, 2 = 백돌
	//int8 piece = 0; // 놓아져 있는 돌이 흑돌인지 백돌인지 체크
	//int8 gameTurn = 1; // 현재 놓을 차례가 흑돌인지 백돌인지 체크
	int32 xPos = 0;
	int32 yPos = 0;

	//bool possiblePiece = false; // 둘수 있는지 없는지 판별
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* OthlloPiece_Button = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* OthlloPiece_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* WhiteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* BlackImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* Preview_WhiteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* Preview_BlackImage;

	UFUNCTION()
	void SetData(int y, int x);
	UFUNCTION()
		void OnBtnClick();

		//void SetGameTurn(int8 turn) { gameTurn = turn; }
		int32 GetX() { return xPos; }
		int32 GetY() { return yPos; }
		//int32 GetPiece() { return piece; }
	void Placement(bool turn);
		//bool GetPossiblePiece() { return possiblePiece; }
	UFUNCTION()
		void PossiblePiece(int8 gameCurrentStatus); // 클릭 가능

		void UnPossiblePiece(); // 클릭 불가능
};