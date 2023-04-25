// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPices_UserWidget.h"
#include "OthelloPlayerController.h"
#include <Components/Button.h>
#include <Components/Image.h>

void UOthelloPices_UserWidget::SetData(int y, int x)
{
	xPos = x;
	yPos = y;
	OthlloPices_Button->OnClicked.AddDynamic(this, &UOthelloPices_UserWidget::OnBtnClick);
}

void UOthelloPices_UserWidget::OnBtnClick()
{
	OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->SetOthelloArrIndex(xPos, yPos);
	if (!bturn && piece == 0)
	{
		bhit = true;
		OthlloPices_Image->SetBrushFromTexture(BlackImage, true);
		piece = 1;
	}
	else if (bturn && piece == 0)
	{
		bhit = true;
		OthlloPices_Image->SetBrushFromTexture(WhiteImage, true);
		piece = 2;
	}
}

void UOthelloPices_UserWidget::ReversePice()
{
	OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (!bturn)
	{
		OthlloPices_Image->SetBrushFromTexture(BlackImage, true);
		piece = 1;
	}
	else if (bturn)
	{
		OthlloPices_Image->SetBrushFromTexture(WhiteImage, true);
		piece = 2;
	}
}

void UOthelloPices_UserWidget::PossiblePice()
{
	possiblePiece = true;
	OthlloPices_Image->SetVisibility(ESlateVisibility::Hidden);

	// 서버일 때 흑돌
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		if (!bturn && piece == 0)
		{
			OthlloPices_Image->SetBrushFromTexture(Preview_BlackImage, true);
			OthlloPices_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

	// 클라 일 때는 백돌!
	if (GetWorld()->GetFirstPlayerController()->GetRemoteRole() == ROLE_Authority)
	{
		if (bturn && piece == 0)
		{
			OthlloPices_Image->SetBrushFromTexture(Preview_WhiteImage, true);
			OthlloPices_Button->SetVisibility(ESlateVisibility::Visible);
			OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}


}

void UOthelloPices_UserWidget::UnPossiblePice()
{
	possiblePiece = false;
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (piece == 0)
	{
		OthlloPices_Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOthelloPices_UserWidget::Changeturn()
{
	bturn = !bturn;
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
}
