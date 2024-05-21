// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricCameraPawn.h"
#include "InteractiveArchController.h"

// Sets default values
AIsometricCameraPawn::AIsometricCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("IsometricSpringArm"));
	SpringArm->TargetArmLength = 2000;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 30.0f;
	SpringArm->AddRelativeRotation(FRotator(-45, 45, 0));
	RootComponent = SpringArm;

	IsometricCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("IsometricCamera"));
	IsometricCamera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	RotationSpeed = 2.0f;
	TargetRotationYawValue = 45.0f;
}

// Called when the game starts or when spawned
void AIsometricCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIsometricCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotationYawValue, CurrentRotation.Roll);
	SpringArm->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * RotationSpeed));
}

// Called to bind functionality to input
void AIsometricCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AInteractiveArchController* PlayerController = Cast<AInteractiveArchController>(GetController());

	IsometricCameraMapping = NewObject<UInputMappingContext>(this);

	CameraLeftRotationAction = NewObject<UInputAction>(this);
	CameraLeftRotationAction->ValueType = EInputActionValueType::Boolean;

	CameraRightRotationAction = NewObject<UInputAction>(this);
	CameraRightRotationAction->ValueType = EInputActionValueType::Boolean;

	CameraZoomInOutAction = NewObject<UInputAction>(this);
	CameraZoomInOutAction->ValueType = EInputActionValueType::Boolean;

	UInputModifierNegate* Negation = NewObject<UInputModifierNegate>(this);

	IsometricCameraMapping->MapKey(CameraLeftRotationAction, EKeys::Q);

	IsometricCameraMapping->MapKey(CameraRightRotationAction, EKeys::E);

	IsometricCameraMapping->MapKey(CameraZoomInOutAction, EKeys::MouseWheelAxis);


	if (EIC) {
		EIC->BindAction(CameraLeftRotationAction, ETriggerEvent::Completed, this, &AIsometricCameraPawn::CameraLeftRotation);
		EIC->BindAction(CameraRightRotationAction, ETriggerEvent::Completed, this, &AIsometricCameraPawn::CameraRightRotation);
		EIC->BindAction(CameraZoomInOutAction, ETriggerEvent::Triggered, this, &AIsometricCameraPawn::CameraZoom);
	}

	if (PlayerController) {
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		//SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(IsometricCameraMapping, 0);
	}
}

void AIsometricCameraPawn::CameraLeftRotation()
{
	TargetRotationYawValue += 45;
	TargetRotationYawValue = FMath::Wrap(TargetRotationYawValue, -180.0f, 180.0f);
}

void AIsometricCameraPawn::CameraRightRotation()
{
	TargetRotationYawValue -= 45;
	TargetRotationYawValue = FMath::Wrap(TargetRotationYawValue, -180.0f, 180.0f);
}

void AIsometricCameraPawn::CameraZoom(const FInputActionValue& ActionValue)
{
	float value = ActionValue.Get<float>();

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 500, 3000);
	SpringArm->TargetArmLength -= value * 20;
}

