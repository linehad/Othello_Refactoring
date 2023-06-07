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

	// �浹, �鵹 �̹���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* WhiteImage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* BlackImage = nullptr;


	// ������ �帧�� ����ϴ� �Լ���
public:
	// ó�� ������ �� ȣ���� �Լ�
	void StartSet();

	// 1���� �迭�� 2���� �迭 ó�� ���� ���� �Լ�
	UFUNCTION()
		inline int16 Board_index(int x, int y) { return (GameInfoStruct.Size * y) + x; }

	void PossiblePiece(const FBoardInfoStruct& Data);

	// ���� ���
	UFUNCTION()
	void Placement(FName EventName, const FBoardInfoStruct& Data);
	void Placement(int32 index, bool turn);
	void Placement(int8 x, int8 y, bool turn);

};