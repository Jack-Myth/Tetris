// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TetrisPawn.generated.h"

UCLASS()
class TETRIS_API ATetrisPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetrisPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* CameraArm;
	UPROPERTY(EditAnywhere)
		UCameraComponent* PawnEye;
	void ViewX(float AxisValue);
	void ViewY(float AxisValue);
	void ViewDistance(float AxisValue);
	void Rot();
	void MoveToLeft();
	void MoveToRight();
	void BeginDropBlock();
	void EndDropBlock();
};
