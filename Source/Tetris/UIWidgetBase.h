// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TetrisPawn.h"
#include "UIWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UUIWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,Category="GameLogic")
	int Score;
	ATetrisPawn* PlayerPawn;
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable,Category="GameLogic")
		void MoveRight();
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
		void MoveLeft();
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
		void DropDown();
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
		void DropDownRealease();
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
		void Rot();
};
