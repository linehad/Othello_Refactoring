// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPices_UserWidget.h"
#include "OthelloPlayerController.h"
#include <Components/Button.h>
#include <Components/Image.h>

void UOthelloPices_UserWidget::SetData(int y, int x)
{
	xPos = x;
	yPos = y;
	OthlloPiece_Button->OnClicked.AddDynamic(this, &UOthelloPices_UserWidget::OnBtnClick);
}

void UOthelloPices_UserWidget::OnBtnClick()
{
	OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->SetOthelloArrIndex(xPos, yPos);
	if (gameTurn=BLACK_TURN && piece == EMPTY)
	{
		bhit = true;
		OthlloPiece_Image->SetBrushFromTexture(BlackImage, true);
		piece = BLACK_PIECE;
	}
	else if (gameTurn = WHITE_TURN && piece == EMPTY)
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

	if (gameTurn=BLACK_TURN)
	{
		OthlloPiece_Image->SetBrushFromTexture(BlackImage, true); // 검은색 돌을 둠
		piece = BLACK_PIECE;
	}
	else if (gameTurn=WHITE_TURN)
	{
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
		if (gameTurn = BLACK_TURN && piece == EMPTY)
		{
			OthlloPiece_Image->SetBrushFromTexture(Preview_BlackImage, true);
			OthlloPiece_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPiece_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

	// 클라 일 때는 백돌!
	if (GetWorld()->GetFirstPlayerController()->GetRemoteRole() == ROLE_Authority)
	{
		if (gameTurn = WHITE_TURN && piece == EMPTY)
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
	gameTurn = !gameTurn;
	OthlloPiece_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
}
