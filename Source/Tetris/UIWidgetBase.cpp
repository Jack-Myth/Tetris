// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris.h"
#include "UIWidgetBase.h"

void UUIWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerPawn = (ATetrisPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void UUIWidgetBase::MoveRight()
{
	if (PlayerPawn)
		PlayerPawn->MoveToRight();
}

void UUIWidgetBase::MoveLeft()
{
	if (PlayerPawn)
		PlayerPawn->MoveToLeft();
}

void UUIWidgetBase::DropDown()
{
	if (PlayerPawn)
		PlayerPawn->BeginDropBlock();
}

void UUIWidgetBase::DropDownRealease()
{
	if (PlayerPawn)
		PlayerPawn->EndDropBlock();
}

void UUIWidgetBase::Rot()
{
	if (PlayerPawn)
		PlayerPawn->Rot();
}
