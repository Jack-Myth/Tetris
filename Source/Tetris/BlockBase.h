// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BlockBase.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ABlockBase : public AActor
{
	GENERATED_BODY()
public:
	ABlockBase();
	UPROPERTY(EditAnywhere, Category = "GameBlock")
		int FallenSpeed=50;
	float FallenDelay=0.5;
	virtual void BeginPlay() override;
	FTimerHandle GoDownTimer;
	void OnReadyGoDown();
	UPROPERTY(VisibleAnywhere, Category = "GameBlock")
		bool IsCurrent=true;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshCollection[4] = {NULL};
	UStaticMeshComponent* RootMesh;
	UMaterialInstanceDynamic* DynamicMI[4] = {NULL};
	void CheckIsLineFull();
};
