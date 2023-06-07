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

	Controller->Server_SetBoardCoordinate(xPos, yPos);
}

void UOthelloPices_UserWidget::Placement(bool turn)
{
	OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (turn == BLACK_TURN)
	{
		OthlloPiece_Image->SetBrushFromTexture(BlackImage, true);
	}
	else if (turn == WHITE_TURN)
	{
		OthlloPiece_Image->SetBrushFromTexture(WhiteImage, true);		
	}
}

void UOthelloPices_UserWidget::PossiblePiece(int8 gameCurrentStatus)
{
	bool GameTurn = !GameStateBase->GetGameTurn();
	if (gameCurrentStatus == SERVER_PLAY|| gameCurrentStatus == SET_START)
	{
		GameTurn = GameStateBase->GetGameTurn();
	}

	OthlloPiece_Image->SetVisibility(ESlateVisibility::Hidden);

	// 서버일 때 흑돌
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		if (GameTurn == BLACK_TURN)
		{
			OthlloPiece_Image->SetBrushFromTexture(Preview_BlackImage, true);
			OthlloPiece_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

	// 클라 일 때는 백돌!
	else if (GetWorld()->GetFirstPlayerController()->GetRemoteRole() == ROLE_Authority)
	{
		if (GameTurn == WHITE_TURN)
		{
			OthlloPiece_Image->SetBrushFromTexture(Preview_WhiteImage, true);
			OthlloPiece_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}


}

void UOthelloPices_UserWidget::UnPossiblePiece()
{
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPiece_Image->SetVisibility(ESlateVisibility::Hidden);
}