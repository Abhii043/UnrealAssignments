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
#include "OrthographicCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API AOrthographicCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AOrthographicCameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Orthographic")
	USpringArmComponent* SpringArm ;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Orthographic")
	UCameraComponent* OrthographicCamera ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic")
	UInputAction* CameraMovementAction;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic")
	UInputAction* CameraZoomInOutAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic")
	UInputAction* CameraXRotationAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic")
	UInputMappingContext* OrthographicCameraMapping;

private:
	UFUNCTION(BlueprintCallable , Category = "Orthographic" , Meta = (AllowPrivateAccess=true))
	void CameraMovement(const FInputActionValue& ActionValue);

	UFUNCTION(BlueprintCallable , Category = "Orthographic" , Meta = (AllowPrivateAccess = true))
	void CameraZoom(const FInputActionValue& ActionValue); 

	UFUNCTION(BlueprintCallable, Category = "Orthographic", Meta = (AllowPrivateAccess = true))
	void RotateX(const FInputActionValue& ActionValue);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Orthographic" ,Meta = (AllowPrivateAccess = true))
	UFloatingPawnMovement* Movement;
};
