// Fill out your copyright notice in the Description page of Project Settings.

#include "Tetris.h"
#include "TetrisPawn.h"
#include "BlockBase.h"
#include "Block_J.h"
#include "Block_O.h"
#include "Block_Z.h"
#include "Block_I.h"
#include "Block_T.h"
#include "UIWidgetBase.h"
#include "../UMG/Public/Blueprint/UserWidget.h"


// Sets default values
ATetrisPawn::ATetrisPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	PawnEye = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnEye"));
	PawnEye->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform);
	PawnEye->SetRelativeLocation(FVector(0, 0, 0));
	PawnEye->Activate(true);
}

// Called when the game starts or when spawned
void ATetrisPawn::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> TargetCollection;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetCollection);
	if (TargetCollection.Num() > 0)
	{
		UClass* ClassToSpawn = NULL;
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
		GetWorld()->SpawnActor(ClassToSpawn, &FTTmp);
	}
#if PLATFORM_ANDROID
	CameraArm->TargetArmLength = 1000;
#endif
	/*UUIWidgetBase* UMGInterface= CreateWidget<UUIWidgetBase>((APlayerController*)GetController(), LoadObject<UUIWidgetBase>(NULL, TEXT("WidgetBlueprint'/Game/Tetris/Misc/UI.UI'"))->StaticClass());
	if (UMGInterface)
		UMGInterface->AddToViewport(0);
	else
		GEngine->AddOnScreenDebugMessage(-1,5, FColor::Red, "CreateWidget Faild");*/
}

// Called every frame
void ATetrisPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("ViewX"),this, &ATetrisPawn::ViewX);
	PlayerInputComponent->BindAxis(TEXT("ViewY"),this, &ATetrisPawn::ViewY);
	PlayerInputComponent->BindAxis(TEXT("ViewDistance"), this, &ATetrisPawn::ViewDistance);
	PlayerInputComponent->BindAction(TEXT("Rot"), EInputEvent::IE_Pressed, this, &ATetrisPawn::Rot);
	PlayerInputComponent->BindAction(TEXT("MoveToLeft"), EInputEvent::IE_Pressed, this, &ATetrisPawn::MoveToLeft);
	PlayerInputComponent->BindAction(TEXT("MoveToRight"), EInputEvent::IE_Pressed, this, &ATetrisPawn::MoveToRight);
	PlayerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Pressed, this, &ATetrisPawn::BeginDropBlock);
	PlayerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Released, this, &ATetrisPawn::EndDropBlock);
}

void ATetrisPawn::ViewX(float AxisValue)
{
	FRotator fR = CameraArm->GetRelativeTransform().GetRotation().Rotator();
#if PLATFORM_ANDROID
	FVector MotionData[4];
	((APlayerController*)GetController())->GetInputMotionState(MotionData[0], MotionData[1], MotionData[2], MotionData[3]);
	CameraArm->SetRelativeRotation(FRotator(fR.Pitch, MotionData[2].X * 9 + 90, 0));
#else
	if (AxisValue * 5 + fR.Yaw < 160 && AxisValue * 5 + fR.Yaw>20)
		CameraArm->SetRelativeRotation(FRotator(fR.Pitch, fR.Yaw + AxisValue * 5, 0));
#endif
}

void ATetrisPawn::ViewY(float AxisValue)
{
	FRotator fR = CameraArm->GetRelativeTransform().GetRotation().Rotator();
#if PLATFORM_ANDROID
	FVector MotionData[4];
	((APlayerController*)GetController())->GetInputMotionState(MotionData[0], MotionData[1], MotionData[2], MotionData[3]);
	CameraArm->SetRelativeRotation(FRotator(MotionData[2].Y * 3, fR.Yaw, 0));
#else
	if (abs(AxisValue * 5 + fR.Pitch) < 75)
		CameraArm->SetRelativeRotation(FRotator(fR.Pitch + AxisValue * 5, fR.Yaw, 0));
#endif
}

void ATetrisPawn::ViewDistance(float AxisValue)
{
	CameraArm->TargetArmLength += AxisValue * 100;
}

void ATetrisPawn::Rot()
{
	TArray<AActor*> BlockCollection;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockBase::StaticClass(), BlockCollection);
	for (AActor* Ac:BlockCollection)
	{
		ABlockBase* ABB = (ABlockBase*)Ac;
		if (ABB->IsCurrent)
		{
			FTransform OriginTransform=ABB->GetActorTransform();
			for (int i=0;i<4;i++)
				ABB->MeshCollection[i]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			ABB->AddActorWorldRotation(FQuat(FRotator(90,0,0)));
			FHitResult HitR;
			ABB->AddActorWorldOffset(FVector(0, 0, -10), true, &HitR);
			ABB->SetActorTransform(OriginTransform);
			//ABB->RootMesh->AddWorldRotation(FQuat(FRotator(-90, 0, 0)));
			for (int i = 0; i < 4; i++)
				ABB->MeshCollection[i]->AttachToComponent(ABB->RootMesh, FAttachmentTransformRules::KeepWorldTransform);
			if (!HitR.bBlockingHit)
				ABB->AddActorWorldRotation(FQuat(FRotator(90, 0, 0)));
		}
	}
}

void ATetrisPawn::MoveToLeft()
{
	TArray<AActor*> BlockCollection;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockBase::StaticClass(), BlockCollection);
	for (AActor* Ac : BlockCollection)
	{
		ABlockBase* ABB = (ABlockBase*)Ac;
		if (ABB->IsCurrent)
		{
			FHitResult HitR;
			FVector OriginLocation = ABB->GetActorLocation();
			ABB->AddActorWorldOffset(FVector(100, 0, 0), true,&HitR);
			if (HitR.bBlockingHit)
				ABB->SetActorLocation(OriginLocation);
		}
	}
}

void ATetrisPawn::MoveToRight()
{
	TArray<AActor*> BlockCollection;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockBase::StaticClass(), BlockCollection);
	for (AActor* Ac : BlockCollection)
	{
		ABlockBase* ABB = (ABlockBase*)Ac;
		if (ABB->IsCurrent)
		{
			FHitResult HitR;
			FVector OriginLocation = ABB->GetActorLocation();
			ABB->AddActorWorldOffset(FVector(-100, 0, 0), true, &HitR);
			if (HitR.bBlockingHit)
				ABB->SetActorLocation(OriginLocation);
		}
	}
}

void ATetrisPawn::BeginDropBlock()
{
	TArray<AActor*> BlockCollection;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockBase::StaticClass(), BlockCollection);
	for (AActor* Ac : BlockCollection)
	{
		ABlockBase* ABB = (ABlockBase*)Ac;
		if (ABB->IsCurrent)
		{
			GetWorldTimerManager().ClearAllTimersForObject(ABB);
			ABB->FallenDelay = 0.05;
			GetWorldTimerManager().SetTimer(ABB->GoDownTimer,ABB,&ABlockBase::OnReadyGoDown, ABB->FallenDelay, true,0);
		}
	}
}

void ATetrisPawn::EndDropBlock()
{
	TArray<AActor*> BlockCollection;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockBase::StaticClass(), BlockCollection);
	for (AActor* Ac : BlockCollection)
	{
		ABlockBase* ABB = (ABlockBase*)Ac;
		if (ABB->IsCurrent)
		{
			GetWorldTimerManager().ClearAllTimersForObject(ABB);
			ABB->FallenDelay = 0.5;
			GetWorldTimerManager().SetTimer(ABB->GoDownTimer,ABB, &ABlockBase::OnReadyGoDown, ABB->FallenDelay, true, 0);
		}
	}
}