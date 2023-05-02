// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPices_UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>

void UOthelloPices_UserWidget::SetData(int y, int x)
{
	m_XPos = x;
	m_YPos = y;
	OthlloPices_Button->OnClicked.AddDynamic(this, &UOthelloPices_UserWidget::OnBtnClick);
}

void UOthelloPices_UserWidget::OnBtnClick()
{
	OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (!b_turn && m_Pice == 0)
	{
		b_hit = true;
		OthlloPices_Image->SetBrushFromTexture(BlackImage, true);
		m_Pice = 1;
	}
	else if (b_turn && m_Pice == 0)
	{
		b_hit = true;
		OthlloPices_Image->SetBrushFromTexture(WhiteImage, true);
		m_Pice = 2;
	}
}

void UOthelloPices_UserWidget::ReversePice()
{
	OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (!b_turn)
	{
		OthlloPices_Image->SetBrushFromTexture(BlackImage, true);
		m_Pice = 1;
	}
	else if (b_turn)
	{
		OthlloPices_Image->SetBrushFromTexture(WhiteImage, true);
		m_Pice = 2;
	}
}

void UOthelloPices_UserWidget::PossiblePice()
{
	m_PossiblePice = true;
	OthlloPices_Image->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (!b_turn && m_Pice == 0)
	{
		OthlloPices_Image->SetBrushFromTexture(Preview_BlackImage, true);
		OthlloPices_Button->SetVisibility(ESlateVisibility::Visible);
	}
	else if (b_turn && m_Pice == 0)
	{
		OthlloPices_Image->SetBrushFromTexture(Preview_WhiteImage, true);
		OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UOthelloPices_UserWidget::UnPossiblePice()
{
	m_PossiblePice = false;
	OthlloPices_Button->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (m_Pice == 0)
	{
		OthlloPices_Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOthelloPices_UserWidget::SetTurn(bool turn)
{
	b_turn = turn;
}

void UOthelloPices_UserWidget::Changeturn()
{
	b_turn = !b_turn;
}
