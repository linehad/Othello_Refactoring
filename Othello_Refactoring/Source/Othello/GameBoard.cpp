#include "GameBoard.h"
#include "ServerGameStateBase.h"
#include "OthelloPlayerController.h"

#include "OthelloPices_UserWidget.h"


#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/GridPanel.h>
#include <Components/UniformGridPanel.h>

void UGameBoard::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = Cast<AServerGameStateBase>(GetWorld()->GetGameState());
	if (GameState) // 스테이트에서 전달해주는 구조체를 통해 착수이벤트 바인딩
	{
		GameState->OnPlacementEvent.AddDynamic(this, &UGameBoard::Placement);
	}
}

void UGameBoard::StartSet()
{
	GameInfoStruct = GameState->GetGameInfoStruct();

	// 버튼에 정보 넘겨주고 함수와 묶기
	if (OthelloButton)
	{
		int index = 0;

		arrOthelloButton.Init(nullptr, GameInfoStruct.Size * GameInfoStruct.Size);
		for (int i = 0; i < GameInfoStruct.Size; i++)
		{
			for (int j = 0; j < GameInfoStruct.Size; j++)
			{
				UUserWidget* widget = CreateWidget(this, OthelloButton); // 버튼 위젯을 만듬
				Board_UniformGridPanel->AddChildToUniformGrid(widget, i, j); // 만들어진 버튼 위젯을 그리드 패널에 추가함
				arrOthelloButton[index] = Cast<UOthelloPices_UserWidget>(widget); // 만들어진 버튼 위젯을 관리하기 위해 배열에 추가함
				arrOthelloButton[index]->SetData(i, j); // 원 클릭 이벤트 바인딩과 버튼의 좌표를 설정함
				arrOthelloButton[index]->UnPossiblePiece(); // 버튼이 클릭 되지 않아야 함 다 비활성화 해줌
				index++;
			}
		}
	}

	// 가운데 돌 두는 기능
	int y = GameInfoStruct.Size / 2;
	int x = (GameInfoStruct.Size / 2 - 1);
	Placement(x, y, BLACK_TURN);
	Placement(x + 1, y - 1, BLACK_TURN);
	Placement(x + 1, y, WHITE_TURN);
	Placement(x, y - 1, WHITE_TURN);
}

void UGameBoard::PossiblePiece(const FBoardInfoStruct& Data)
{
	// 착수 가능한 위치를 표시함
	for (int i = 0; i < Data.PlacementPosArr.Num(); i++)
	{
		int index = Data.PlacementPosArr[i];
		if (GetWorld()->GetFirstPlayerController()->HasAuthority())
		{
			arrOthelloButton[index]->PossiblePiece(SERVER_PLAY);
		}
		else
		{
			arrOthelloButton[index]->PossiblePiece(CLIENT_PLAY);
		}
	}
}

void UGameBoard::Placement(FName EventName, const FBoardInfoStruct& Data)
{
	const int32 ClickPos = Data.Index;

	for (int i = 0; i < LastBoardInfo.PlacementPosArr.Num(); i++)
	{
		int32 index = LastBoardInfo.PlacementPosArr[i];
		if (index != ClickPos)
		{
			arrOthelloButton[index]->UnPossiblePiece();
		}
	}
	if (EventName == TEXT("Placement"))
	{
		Placement(Data.Index, !Data.Turn); // GameMode에서 미리 턴을 뒤집었기 때문에 현재 턴은 다르다!

		// 착수 위치에 기반해서 돌을 실제로 뒤집어줌
		for(int i=0;i< Data.ReversePosArr.Num();i++)
		{
			int index = Data.ReversePosArr[i];
			arrOthelloButton[index]->Placement(!Data.Turn);
		}

		// 착수 가능한 위치를 표시함
		PossiblePiece(Data);
	}
	// 제한시간이 다 지나가면 실행됨
	else if (EventName == TEXT("TimeOut"))
	{
		// 착수 가능한 위치를 표시함
		PossiblePiece(Data);
	}
	else if (EventName == TEXT("TurnOut")) // 더 이상 둘 수 없기에 그냥 턴이 넘어갔다. 따라서 미리 뒤집는 과정이 없어짐
	{
		// 착수 가능한 위치를 표시함
		for (int i = 0; i < Data.PlacementPosArr.Num(); i++)
		{
			int index = Data.PlacementPosArr[i];
			arrOthelloButton[index]->PossiblePiece(SERVER_PLAY);
		}
	}
	// 게임 시작시 최초로 실행됨
	else if (EventName == TEXT("SetStart"))
	{
		for (int i = 0; i < Data.PlacementPosArr.Num(); i++)
		{
			int index = Data.PlacementPosArr[i];
			arrOthelloButton[index]->PossiblePiece(SET_START);
		}
	}
	LastBoardInfo = Data;
}

void UGameBoard::Placement(int32 index, bool turn)
{
	arrOthelloButton[index]->Placement(turn);
}

void UGameBoard::Placement(int8 x, int8 y, bool turn)
{
	arrOthelloButton[Board_index(x, y)]->Placement(turn);
}