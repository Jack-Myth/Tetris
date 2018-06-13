// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "SingleBlock.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ASingleBlock : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ASingleBlock();
	virtual void Tick(float DeltaSeconds) override;
	bool ShouldNotifyDrop = false;
};
