// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UPawnAttributeAsset.h"
#include "AFirstPersonPawn.generated.h"

UCLASS()
class ASSIGNMENT_1_2_API AAFirstPersonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAFirstPersonPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const struct FInputActionValue&  ActionValue);
	void Look(const FInputActionValue&  ActionValue);

	UPROPERTY(VisibleDefaultsOnly , category = "Pawn")
	UCapsuleComponent* CollisonCapsule;

	UPROPERTY(EditAnywhere, category = "Pawn")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, category = "Pawn")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, category = "Pawn")
	UInputMappingContext* PawnInputmappingContext;

	UPROPERTY(EditDefaultsOnly, category = "Pawn")
	UInputAction* MovementAction;
	
	UPROPERTY(EditDefaultsOnly, category = "Pawn")
	UFloatingPawnMovement* FloatingMovement;

	UPROPERTY(EditAnywhere, category = "Pawn")
	float MoveScale;

	UPROPERTY()
	UUPawnAttributeAsset* DataAsset;
};
