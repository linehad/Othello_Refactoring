#include "OthelloPices_UserWidget.h"
#include "OthelloPlayerController.h"
#include "ServerGameStateBase.h"

#include <Components/Button.h>
#include <Components/Image.h>

void UOthelloPices_UserWidget::SetData(int y, int x)
{
	xPos = x;
	yPos = y;
	OthlloPiece_Button->OnClicked.AddDynamic(this, &UOthelloPices_UserWidget::OnBtnClick);
	GameStateBase = Cast<AServerGameStateBase>(GetWorld()->GetGameState());
}

void UOthelloPices_UserWidget::OnBtnClick()
{
	AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());

	OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	Controller->SetBoardCoordinate(xPos, yPos);

	if (Controller->HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("server"));
	}
	else
	{
		bool aaturn = GameStateBase->GetGameTurn();
		UE_LOG(LogTemp, Log, TEXT("client"));
	}

	if (GameStateBase->GetGameTurn() == BLACK_TURN && piece == EMPTY)
	{
		bhit = true;
		OthlloPiece_Image->SetBrushFromTexture(BlackImage, true);
		piece = BLACK_PIECE;
	}
	else if (GameStateBase->GetGameTurn() == WHITE_TURN && piece == EMPTY)
	{
		bhit = true;
		OthlloPiece_Image->SetBrushFromTexture(WhiteImage, true);
		piece = WHITE_PIECE;
	}
}

void UOthelloPices_UserWidget::ReversePiece()
{
	OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (GameStateBase->GetGameTurn() == BLACK_TURN)
	{
		OthlloPiece_Image->SetBrushFromTexture(BlackImage, true); // 검은색 돌을 둠
		piece = BLACK_PIECE;
	}
	else if (GameStateBase->GetGameTurn() == WHITE_TURN)
	{
		OthlloPiece_Image->SetBrushFromTexture(WhiteImage, true);
		piece = WHITE_PIECE;
	}
}

void UOthelloPices_UserWidget::StartPlacement(bool turn)
{
	OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (turn == BLACK_TURN && piece == EMPTY)
	{
		bhit = true;
		OthlloPiece_Image->SetBrushFromTexture(BlackImage, true);
		piece = BLACK_PIECE;
	}
	else if (turn == WHITE_TURN && piece == EMPTY)
	{
		bhit = true;
		OthlloPiece_Image->SetBrushFromTexture(WhiteImage, true);
		piece = WHITE_PIECE;
	}
}

void UOthelloPices_UserWidget::PossiblePiece()
{
	possiblePiece = true;
	OthlloPiece_Image->SetVisibility(ESlateVisibility::Hidden);

	// 서버일 때 흑돌
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		if (GameStateBase->GetGameTurn() == BLACK_TURN && piece == EMPTY)
		{
			OthlloPiece_Image->SetBrushFromTexture(Preview_BlackImage, true);
			OthlloPiece_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

	// 클라 일 때는 백돌!
	if (GetWorld()->GetFirstPlayerController()->GetRemoteRole() == ROLE_Authority)
	{
		if (GameStateBase->GetGameTurn() == WHITE_TURN && piece == EMPTY)
		{
			OthlloPiece_Image->SetBrushFromTexture(Preview_WhiteImage, true);
			OthlloPiece_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}


}

void UOthelloPices_UserWidget::UnPossiblePiece()
{
	possiblePiece = false;
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (piece == EMPTY) // 비어있으면 이미지 숨김
	{
		OthlloPiece_Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOthelloPices_UserWidget::ChangeTurn()
{
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
}