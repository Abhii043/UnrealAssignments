// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PespectiveCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API APespectiveCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	APespectiveCameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Perspective")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Perspective")
	UCameraComponent* PerspectiveCamera;

	UPROPERTY(VisibleDefaultsOnly, category = "Perspective")
	UInputMappingContext* PawnInputmappingContext;

	UPROPERTY(EditDefaultsOnly, category = "Perspective")
	UInputAction* MovementAction; 
	
	UPROPERTY(EditDefaultsOnly, category = "Perspective")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, category = "Perspective")
	UFloatingPawnMovement* FloatingMovement;

	UPROPERTY(EditAnywhere, category = "Perspective")
	float MoveScale;

private:

	UFUNCTION(BlueprintCallable, Category = "Perspective", Meta = (AllowPrivateAccess = true))
	void Move(const struct FInputActionValue&  ActionValue);

	UFUNCTION(BlueprintCallable, Category = "Perspective", Meta = (AllowPrivateAccess = true))
	void Look(const FInputActionValue&  ActionValue);
};
