// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthographicCameraPawn.h"
#include "InteractiveArchController.h"

// Sets default values
AOrthographicCameraPawn::AOrthographicCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("OrthographicSpringArm"));
	SpringArm->TargetArmLength = 0;
	SpringArm->TargetOffset = FVector(0, 0, 2000);
	RootComponent = SpringArm;

	OrthographicCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OrthographicCamera"));
	OrthographicCamera->SetupAttachment(SpringArm);
	OrthographicCamera->AddRelativeRotation(FRotator(-90.0f,0,0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AOrthographicCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOrthographicCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrthographicCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AInteractiveArchController* PlayerController = Cast<AInteractiveArchController>(GetController());

	OrthographicCameraMapping = NewObject<UInputMappingContext>(this);

	CameraMovementAction = NewObject<UInputAction>(this);
	CameraMovementAction->ValueType = EInputActionValueType::Axis2D;

	CameraXRotationAction = NewObject<UInputAction>(this);
	CameraXRotationAction->ValueType = EInputActionValueType::Axis3D;

	CameraZoomInOutAction = NewObject<UInputAction>(this);
	CameraZoomInOutAction->ValueType = EInputActionValueType::Boolean;

	UInputModifierNegate* Negation = NewObject<UInputModifierNegate>(this);

	FEnhancedActionKeyMapping& ForwardMovementmapping = OrthographicCameraMapping->MapKey(CameraMovementAction, EKeys::W);

	FEnhancedActionKeyMapping& BackwardMovementmapping = OrthographicCameraMapping->MapKey(CameraMovementAction, EKeys::S);
	BackwardMovementmapping.Modifiers.Add(Negation);
	
	UInputModifierSwizzleAxis* RightSwizzle = NewObject<UInputModifierSwizzleAxis>(this);
	RightSwizzle->Order = EInputAxisSwizzle::YXZ;

	FEnhancedActionKeyMapping& RightMovementmapping = OrthographicCameraMapping->MapKey(CameraMovementAction, EKeys::D);
	RightMovementmapping.Modifiers.Add(RightSwizzle);

	FEnhancedActionKeyMapping& LeftMovementmapping = OrthographicCameraMapping->MapKey(CameraMovementAction, EKeys::A);
	LeftMovementmapping.Modifiers.Add(RightSwizzle);
	LeftMovementmapping.Modifiers.Add(Negation);

	OrthographicCameraMapping->MapKey(CameraZoomInOutAction, EKeys::MouseWheelAxis);

	auto& MouseXRotation = OrthographicCameraMapping->MapKey(CameraXRotationAction, EKeys::MouseX);
	MouseXRotation.Modifiers.Add(RightSwizzle);

	if (EIC) {
		EIC->BindAction(CameraMovementAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::CameraMovement);
		EIC->BindAction(CameraZoomInOutAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::CameraZoom);
		EIC->BindAction(CameraXRotationAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::RotateX);
	}

	if (PlayerController) {
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		//SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(OrthographicCameraMapping, 0);
	}
}

void AOrthographicCameraPawn::CameraMovement(const FInputActionValue& ActionValue)
{
	FVector AxisAction = ActionValue.Get<FVector>();
	AddMovementInput(GetActorRotation().RotateVector(AxisAction) , 2.0f);
}

void AOrthographicCameraPawn::CameraZoom(const FInputActionValue& ActionValue)
{
	float value = ActionValue.Get<float>();
	
	SpringArm->TargetOffset.Z -= value * 20;
	SpringArm->TargetOffset.Z = FMath::Clamp(SpringArm->TargetOffset.Z, 500 , 10000);

}

void AOrthographicCameraPawn::RotateX(const FInputActionValue& ActionValue)
{
	FRotator value = FRotator(ActionValue[0],ActionValue[1],ActionValue[2]);
	value *= GetWorld()->GetDeltaSeconds() * 50;
	value += GetActorRotation() * 2;
	SetActorRotation(value);
}

