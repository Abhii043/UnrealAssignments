
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "TopDownPawn.generated.h"

//DECLARE_DELEGATE_ThreeParams(ShowCameraParam , FString , FString, FString)

UCLASS()
class ASSIGNMENT_3_4_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "MyTopDownPawn")
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "MyTopDownPawn")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "MyTopDownPawn")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly,Category = "MyTopDownPawn")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly,Category = "MyTopDownPawn")
	UInputAction* MoveAction;  

	UPROPERTY(EditDefaultsOnly,Category = "MyTopDownPawn")
	UInputAction* ScrollAction;
	
	UPROPERTY(EditDefaultsOnly , Category = "MyTopDownPawn")
	UFloatingPawnMovement* FloatingMovement;

	UFUNCTION(BlueprintCallable, Category = "MyTopDownPawn")
	void Move(const FInputActionValue& ActionValue);
	
	UFUNCTION(BlueprintCallable, Category = "MyTopDownPawn")
	void ZoomInOut(const FInputActionValue& ActionValue);

	//UFUNCTION(BLueprintimplementableEvent)
	//void ShowNotification(const FString& Offst_Z , const FString& ArmLength, const FString& Angle);

	//ShowCameraParam NotificationMessage;
};
