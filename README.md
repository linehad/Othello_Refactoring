# [ 목차 ]<a name='0'></a>
#### 1. [개요](#1)
#### 2. [지적사항](#2)
#### 3. [간단한 계획](#3)
#### 4. [개발과정]
1. [작업 1주차](#4)
2. [작업 2주차](#5)
3. [작업 3주차](#6)
4. [작업 4주차](#7)
5. [작업 5주차](#8)
6. [작업 6주차](#9)
7. [작업 7주차](#10)
8. [작압 8주차](#11)

# 개요<a name='1'></a>
 [목차로 돌아가기](#0)<br>
 
 https://github.com/linehad/Ue4_OthelloAi
 <br>
 언리얼 UMG에 대한 개념이 없을 무렵 이를 학습하기 위해 UMG만을 사용해서 오델로를 만들고 Ai를 추가하였습니다.<br>
 하지만 코드 리뷰 당시 GameBoard 클래스의 크기가 너무 비대하다는 지적을 받았고<br>
 이를 해결하면서 만든 오델로를 멀티게임으로 바꿔보라는 과제를 받았습니다.<br>
 
 https://github.com/linehad/Ue4_OthelloMultiGame
 <br>
 하지만 GameBoard의 크기가 비대하다는 것의 뜻을 잘못 이해했고, 코드의 양을 줄이기 위해 컨트롤러, 스테이트 등 코드를 옮기기만 했습니다.<br>
 이동 로직을 GameMode로 옮겼어야 됐다는 것을 깨닫고 리펙토링을 해보려고 합니다.<br>
 
 # 지적사항<a name='2'></a>
 [목차로 돌아가기](#0)<br>
 
 지적사항은 다음과 같습니다.
 1. GameBoard.cpp의 클래스가 비대하다
 2. 하드코딩 되어있다.
 3. 변수의 이름이 직관적이지 않다.
 4. 클래스를 나누는 연습이 필요하다.
 <br>
 위 4가지 점을 유의 하면서 리팩토링을 합니다. <br>
 
 # 간단한 계획<a name='3'></a>
 [목차로 돌아가기](#0)<br>

1. board에 있는 점수, 타이머 위젯을 별도의 클래스로 나눕니다.
2. 이동 로직을 게임 모드로 옮깁니다.
3. Ai와 멀티플레이 게임을 합쳐서 솔로플레이, 멀티 플레이 기능으로 나눈다.
4. 멀티플레이의 경우 RPC의 구조를 모를 때 작성해서 불안정하다. 게임모드에서 처리를 담당하므로, 클라에서는 착수 위치만 넘겨 주게 만든다.

이상 4가지 점을 유의 하면서 수정합니다.

 # 1주차<a name='4'></a>
 [목차로 돌아가기](#0)<br>
 
1주차 목표<br>
여기저기로 분산 된 코드 다시 합치기<br>
보드는 보드판을 두면 말을 두는 기능만 넣어둘 것!<br><br>

OthelloGameModeBase.cpp<br>
<img src="https://user-images.githubusercontent.com/91234912/234196379-29657deb-61a0-4d95-91e2-9f8cac2959a2.png" width="800"><br><br>
코드의 로직을 전체적으로 흩뿌려 놨는데 이를 일단 모드에 모아놓습니다.<br>
아직까지는 강제로 옮겼기 때문에 오류 투성이인 코드이지만 하나씩 수정해 나갑니다.<br><br>

MainMenu.cpp<br>
```c++
void UMainMenu::StartButtonCallback()
{	
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		Cast<AOthelloGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetGameData(size, time); // 게임 모드로 변경
 	 	// Cast<AServerGameStateBase>(GetWorld()->GetGameState())->SetGameData(size, time); 기존 코드
	}

	for (auto Iter = GetWorld()->GetControllerIterator(); Iter; ++Iter) 
	{
		Cast<AOthelloPlayerController>(Iter->Get())->ChangeWidget(BoardWidgetClass);
	}
}
```

<br>
게임의 흐름에 맞춰서 코드를 수정합니다.<br>
PostLogin을 통해 2명의 유저가 접속하면 MainMenu 위젯으로 넘어가게 됩니다.<br>
기존에는 GameState로 보드의 정보와 제한시간을 넘겼지만 게임의 전반적인 연산을 게임 모드에서 할 것이기 때문에 처음부터 모드로 넘깁니다. 하지만 위젯은 리플리케이션이 되지 않기 때문에 보드는 역시 자신의 크기를 알아 합니다.<br><br>


GameBoard.cpp<br>
UGameBoard::NativeConstruct()<br>
```c++
	boardSize = Cast<AServerGameStateBase>(GetWorld()->GetGameState())->GetBoardSize();
	limitTime = Cast<AServerGameStateBase>(GetWorld()->GetGameState())->GetLimitTime();

	seconds = limitTime;
	// 버튼에 정보 넘겨주고 함수와 묶기
	if (OthelloButton)
	{
		int arr_index = 0;

		arrOthelloButton.Init(nullptr, boardSize * boardSize);
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				UUserWidget* widget = CreateWidget(this, OthelloButton);
				Board_UniformGridPanel->AddChildToUniformGrid(widget, i, j);
				arrOthelloButton[arr_index] = Cast<UOthelloPices_UserWidget>(widget);
				arrOthelloButton[arr_index]->SetData(i, j);
				arrOthelloButton[arr_index]->OthlloPices_Button->OnClicked.AddDynamic(this, &UGameBoard::NextTurn);
				arr_index++;
			}
		}
		// 시작 돌 놓기
		StartSet();
	}
```
<br>
BeginePlay와 같은 역할을 하는 NativeConstruct는 오델로 보드가 생성될 때 보드의 크기와 제한시간을 게임 스테이트에서 받아서 생성하고 있습니다.<br>
이렇게 할 경우 보드가 만들어지는 타이밍이 보드 크기와 타이머를 받는 시간보다 늦어진다면, 오류가 생길것이 자명하므로 이들을 따로 빼서 함수로 만들고 RPC를 통해 인자를 넘겨서 만들게 하겠습니다.<br>
