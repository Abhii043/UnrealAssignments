// Fill out your copyright notice in the Description page of Project Settings.


#include "AFirstPersonPawn.h"

// Sets default values
AAFirstPersonPawn::AAFirstPersonPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisonCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisonCapsual"));
	CollisonCapsule->SetCapsuleHalfHeight(90);
	CollisonCapsule->SetCapsuleRadius(40);
	CollisonCapsule->SetEnableGravity(false);
	RootComponent = CollisonCapsule;
	CollisonCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	CollisonCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); 

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); 
	SpringArm->SetupAttachment(CollisonCapsule);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm , USpringArmComponent::SocketName); 

	//For Movement of Pawn
	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement"));
	MoveScale = 1.0f;

	DataAsset = LoadObject<UUPawnAttributeAsset>(this, TEXT("/Script/Assignment_1_2.UPawnAttributeAsset'/Assignment_1_2/Assignment_2/Assets/FPPAsset.FPPAsset'"));
}

// Called when the game starts or when spawned
void AAFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

// Called to bind functionality to input
void AAFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	PawnInputmappingContext = NewObject<UInputMappingContext>(this);

	//MovementAction Mapping
	MovementAction = NewObject<UInputAction>(this);
	MovementAction->ValueType = EInputActionValueType::Axis3D;

	UInputModifierNegate* MovementActionNegateModifier = NewObject<UInputModifierNegate>(this);
	MovementActionNegateModifier->bX = true;
	MovementActionNegateModifier->bY = true;
	MovementActionNegateModifier->bZ = true;

	FEnhancedActionKeyMapping& MovementForwardActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::W);

	FEnhancedActionKeyMapping& MovementBackwardActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::S);
	MovementBackwardActionMapping.Modifiers.Add(MovementActionNegateModifier);

	UInputModifierSwizzleAxis* MovementActionSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>(this);
	MovementActionSwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;
	FEnhancedActionKeyMapping& MovementRightActionMapping  = PawnInputmappingContext->MapKey(MovementAction, EKeys::D);
	MovementRightActionMapping.Modifiers.Add(MovementActionSwizzleAxisModifier);

	FEnhancedActionKeyMapping& MovementLeftActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::A);
	MovementLeftActionMapping.Modifiers.Add(MovementActionSwizzleAxisModifier);
	MovementLeftActionMapping.Modifiers.Add(MovementActionNegateModifier);

	UInputModifierSwizzleAxis* MovementUpDownActionSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>(this); 
	MovementUpDownActionSwizzleAxisModifier->Order = EInputAxisSwizzle::ZYX;
	FEnhancedActionKeyMapping& MovementUpActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::Q);
	MovementUpActionMapping.Modifiers.Add(MovementUpDownActionSwizzleAxisModifier);
	 
	FEnhancedActionKeyMapping& MovementDownActionMapping = PawnInputmappingContext->MapKey(MovementAction, EKeys::E);
	MovementDownActionMapping.Modifiers.Add(MovementUpDownActionSwizzleAxisModifier); 
	MovementDownActionMapping.Modifiers.Add(MovementActionNegateModifier); 

	EnhancedInput->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AAFirstPersonPawn::Move);



	//Mouse Movement
	UInputAction* LookAction = NewObject<UInputAction>(this);
	LookAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* LookActionMouseNegateModifier = NewObject<UInputModifierNegate>(this);
	LookActionMouseNegateModifier->bX = false;
	LookActionMouseNegateModifier->bY = true;
	LookActionMouseNegateModifier->bZ = false;

	FEnhancedActionKeyMapping& LookActionMapping = PawnInputmappingContext->MapKey(LookAction, EKeys::Mouse2D);
	LookActionMapping.Modifiers.Add(LookActionMouseNegateModifier);

	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered , this , &AAFirstPersonPawn::Look);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {

		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		check(LocalPlayer);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); 
		check(Subsystem); 

		//Subsystem->ClearAllMappings(); 
		Subsystem->AddMappingContext(PawnInputmappingContext, 0);  
	}
}

void AAFirstPersonPawn::Move(const FInputActionValue& ActionValue)
{
	FVector MovementInput = ActionValue.Get<FInputActionValue::Axis3D>();
	AddMovementInput(GetControlRotation().RotateVector(MovementInput), MoveScale);
}

void AAFirstPersonPawn::Look(const FInputActionValue& ActionValue)
{
	FVector2D LookAxisVector = ActionValue.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X); 
	AddControllerPitchInput(LookAxisVector.Y);
}

