// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris.h"
#include "SingleBlock.h"

ASingleBlock::ASingleBlock()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetMobility(EComponentMobility::Movable);
}

void ASingleBlock::Tick(float DeltaSeconds)
{
	FVector Scale = GetActorScale();
	if (Scale.X <= 0)
	{
		if (ShouldNotifyDrop)
		{
			TArray<AActor*> SingleB;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASingleBlock::StaticClass(), SingleB);
			for (AActor* B:SingleB)
			{
				if (B->GetActorLocation().Z>GetActorLocation().Z)
					B->SetActorLocation(B->GetActorLocation() - FVector(0, 0, 100));
			}
		}
		Destroy();
	}
	SetActorScale3D(Scale - FVector(0.04));
}