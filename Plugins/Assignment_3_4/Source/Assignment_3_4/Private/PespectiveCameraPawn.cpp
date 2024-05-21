// Fill out your copyright notice in the Description page of Project Settings.


#include "PespectiveCameraPawn.h"
#include "InteractiveArchController.h"

// Sets default values
APespectiveCameraPawn::APespectiveCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Perspective SpringArm"));
	SpringArm->TargetArmLength = 0;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->TargetOffset = FVector(-400,0,100);
	RootComponent = SpringArm;
	SpringArm->bUsePawnControlRotation = true;

	PerspectiveCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Perspective Camera"));
	PerspectiveCamera->SetupAttachment(SpringArm); 

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement"));
	MoveScale = 1.5f;
}

// Called when the game starts or when spawned
void APespectiveCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APespectiveCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APespectiveCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PawnInputmappingContext = NewObject<UInputMappingContext>(this);

	MovementAction = NewObject<UInputAction>(this);
	MovementAction->ValueType = EInputActionValueType::Axis3D;

	//Negation Modifier
	UInputModifierNegate* MovementActionNegateModifier = NewObject<UInputModifierNegate>(this);

	FEnhancedActionKeyMapping& MovementForwardActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::W);

	FEnhancedActionKeyMapping& MovementBackwardActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::S);
	MovementBackwardActionMapping.Modifiers.Add(MovementActionNegateModifier);

	//SwizzleAxis For Left & Right Movement
	UInputModifierSwizzleAxis* MovementActionSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>(this);
	MovementActionSwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;

	FEnhancedActionKeyMapping& MovementRightActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::D);
	MovementRightActionMapping.Modifiers.Add(MovementActionSwizzleAxisModifier);

	FEnhancedActionKeyMapping& MovementLeftActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::A);
	MovementLeftActionMapping.Modifiers.Add(MovementActionSwizzleAxisModifier);
	MovementLeftActionMapping.Modifiers.Add(MovementActionNegateModifier);

	//SwizzleAxis For Up & Down Movement
	UInputModifierSwizzleAxis* MovementUpDownActionSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>(this);
	MovementUpDownActionSwizzleAxisModifier->Order = EInputAxisSwizzle::ZYX;
	FEnhancedActionKeyMapping& MovementUpActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::E);
	MovementUpActionMapping.Modifiers.Add(MovementUpDownActionSwizzleAxisModifier);

	FEnhancedActionKeyMapping& MovementDownActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::Q);
	MovementDownActionMapping.Modifiers.Add(MovementUpDownActionSwizzleAxisModifier);
	MovementDownActionMapping.Modifiers.Add(MovementActionNegateModifier);

	//Mouse Movement
	LookAction = NewObject<UInputAction>(this);
	LookAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* LookActionMouseNegateModifier = NewObject<UInputModifierNegate>(this);
	LookActionMouseNegateModifier->bX = false;
	LookActionMouseNegateModifier->bY = true;
	LookActionMouseNegateModifier->bZ = false;

	FEnhancedActionKeyMapping& LookActionMapping = PawnInputmappingContext->MapKey(LookAction, EKeys::Mouse2D);
	LookActionMapping.Modifiers.Add(LookActionMouseNegateModifier);

	//Binding Action With its Function 
	EIC->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APespectiveCameraPawn::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APespectiveCameraPawn::Look);

	if (AInteractiveArchController* PlayerController = Cast<AInteractiveArchController>(GetController())) {

		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		check(LocalPlayer);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(Subsystem);

		Subsystem->ClearAllMappings(); 
		Subsystem->AddMappingContext(PawnInputmappingContext, 0);
	}
}

void APespectiveCameraPawn::Move(const FInputActionValue& ActionValue)
{
	FVector Value = ActionValue.Get<FInputActionValue::Axis3D>();
	FVector MovementInput = ActionValue.Get<FVector>();
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

	AddMovementInput(ForwardDirection, MovementInput.X);
	AddMovementInput(RightDirection, MovementInput.Y);
	AddMovementInput(UpDirection, MovementInput.Z);
}

void APespectiveCameraPawn::Look(const FInputActionValue& ActionValue)
{
	FVector2D MouseAxisVal = ActionValue.Get<FVector2D>();

	AddControllerYawInput(MouseAxisVal.X);
	AddControllerPitchInput(MouseAxisVal.Y);
}

