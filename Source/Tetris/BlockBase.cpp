// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris.h"
#include "BlockBase.h"
#include "Engine/TargetPoint.h"
#include "Block_J.h"
#include "Block_O.h"
#include "Block_Z.h"
#include "Block_I.h"
#include "SingleBlock.h"
#include "Block_T.h"

ABlockBase::ABlockBase()
{
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootMesh->SetVisibility(false);
	RootComponent = RootMesh;
	MeshCollection[0] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0"));
	MeshCollection[1] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	MeshCollection[2] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	MeshCollection[3] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3"));
	for (UStaticMeshComponent* MeshC : MeshCollection)
	{
		MeshC->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
		MeshC->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object);
		MeshC->SetMaterial(0, ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Tetris/Materials/BaseMaterial.BaseMaterial'")).Object);
		MeshC->SetRelativeScale3D(FVector(0.95, 0.95, 0.95));
	}
}

void ABlockBase::BeginPlay()
{
	for (int i = 0; i < 4; i++)
		(DynamicMI[i]=MeshCollection[i]->CreateDynamicMaterialInstance(0))->SetVectorParameterValue(
			TEXT("BaseColor"), FLinearColor(FMath::RandRange(0.0f, 1.0f)*50, FMath::RandRange(0.0f, 1.0f)*50, FMath::RandRange(0.0f, 1.0f)*50,0));
	GetWorldTimerManager().SetTimer(GoDownTimer,this,&ABlockBase::OnReadyGoDown, FallenDelay,true, FallenDelay);
}

void ABlockBase::OnReadyGoDown()
{
	FVector OriginLocation = GetActorLocation();
	FHitResult HitR;
	AddActorWorldOffset(FVector(0, 0, -FallenSpeed), true,&HitR);
	if (HitR.bBlockingHit)
	{
		SetActorLocation(OriginLocation);
		TArray<AActor*> TargetCollection;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetCollection);
		if (TargetCollection.Num()>0)
		{
			GetWorldTimerManager().ClearAllTimersForObject(this);
			IsCurrent = false;
			UClass* ClassToSpawn=NULL;
			switch (FMath::RandRange(0, 4))
			{
			case 0:
				ClassToSpawn = ABlock_J::StaticClass();
				break;
			case 1:
				ClassToSpawn = ABlock_O::StaticClass();
				break;
			case 2:
				ClassToSpawn = ABlock_Z::StaticClass();
				break;
			case 3:
				ClassToSpawn = ABlock_I::StaticClass();
				break;
			case 4:
				ClassToSpawn = ABlock_T::StaticClass();
				break;
			}
			FTransform FTTmp = TargetCollection[0]->GetActorTransform();
			FActorSpawnParameters FASP;
			FASP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
			SetActorEnableCollision(false);
			for (int i = 0; i < 4; i++)
			{
				FLinearColor LineColorValue;
				ASingleBlock* SingleMesh;
				DynamicMI[i]->GetVectorParameterValue(TEXT("BaseColor"), LineColorValue);
				LineColorValue/=FLinearColor(3,3,3,3);
				DynamicMI[i]->SetVectorParameterValue(TEXT("BaseColor"), LineColorValue);
				SingleMesh=GetWorld()->SpawnActor<ASingleBlock>(MeshCollection[i]->GetComponentLocation(), MeshCollection[i]->GetComponentRotation());
				SingleMesh->GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
				SingleMesh->SetActorScale3D(FVector(0.95, 0.95, 0.95));
				SingleMesh->GetStaticMeshComponent()->SetMaterial(0, DynamicMI[i]);
				//SingleMesh->GetStaticMeshComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
			}
			CheckIsLineFull();
			if (!GetWorld()->SpawnActor(ClassToSpawn, &FTTmp, FASP))
			{
				GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Blue, "Game Over!");
			}
			Destroy();
		}
	}
}

void ABlockBase::CheckIsLineFull()
{
	TArray<UStaticMeshComponent> BlockElement;
	for (int i=100;i<1600;i+=100)
	{
		TArray<AActor* >OverlapingActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> EOTQ;
		EOTQ.Add((EObjectTypeQuery)ECollisionChannel::ECC_Destructible);
		EOTQ.Add((EObjectTypeQuery)ECollisionChannel::ECC_PhysicsBody);
		EOTQ.Add((EObjectTypeQuery)ECollisionChannel::ECC_WorldDynamic);
		EOTQ.Add((EObjectTypeQuery)ECollisionChannel::ECC_WorldStatic);
		UKismetSystemLibrary::BoxOverlapActors(GetWorld(), FVector(0, 0, i), FVector(850, 10, 10), EOTQ, ASingleBlock::StaticClass(), TArray<AActor*>(), OverlapingActors);
		if (OverlapingActors.Num() == 10)
		{
			((ASingleBlock*)OverlapingActors[0])->ShouldNotifyDrop = true;
			for (AActor* Sb:OverlapingActors)
				Sb->SetActorTickEnabled(true);
		}
	}
}