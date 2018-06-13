// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris.h"
#include "Block_O.h"

ABlock_O::ABlock_O()
{
	((UStaticMeshComponent*)RootComponent)->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Tetris/Meshs/Block_O.Block_O'")).Object);
	MeshCollection[0]->SetRelativeLocation(FVector(50, 0, -50));
	MeshCollection[1]->SetRelativeLocation(FVector(50, 0, 50));
	MeshCollection[2]->SetRelativeLocation(FVector(-50, 0, -50));
	MeshCollection[3]->SetRelativeLocation(FVector(-50, 0, 50));
}