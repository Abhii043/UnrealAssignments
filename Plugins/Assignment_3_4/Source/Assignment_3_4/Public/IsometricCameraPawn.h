// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <InputMappingContext.h>
#include <InputAction.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "IsometricCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API AIsometricCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AIsometricCameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Isometric")
	USpringArmComponent* SpringArm ;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Isometric")
	UCameraComponent* IsometricCamera ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Isometric")
	UInputAction* CameraLeftRotationAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Isometric")
	UInputAction* CameraRightRotationAction;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Isometric")
	UInputAction* CameraZoomInOutAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic")
	UInputMappingContext* IsometricCameraMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Orthographic")
	float RotationSpeed ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Orthographic")
	float TargetRotationYawValue;

	UPROPERTY()
	int count = 1;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic" ,Meta = (AllowPrivateAccess = true))
	UFloatingPawnMovement* Movement;

	UFUNCTION(BlueprintCallable , Category = "Orthographic" , Meta = (AllowPrivateAccess=true))
	void CameraLeftRotation();

	UFUNCTION(BlueprintCallable , Category = "Orthographic" , Meta = (AllowPrivateAccess=true))
	void CameraRightRotation();

	UFUNCTION(BlueprintCallable , Category = "Orthographic" , Meta = (AllowPrivateAccess = true))
	void CameraZoom(const FInputActionValue& ActionValue); 
};
