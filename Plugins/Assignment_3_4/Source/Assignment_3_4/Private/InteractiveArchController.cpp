// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

AInteractiveArchController::AInteractiveArchController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	IsDestroyed = true;

	PawnReference.Add(AOrthographicCameraPawn::StaticClass());
	PawnReference.Add(APespectiveCameraPawn::StaticClass());
	PawnReference.Add(AIsometricCameraPawn::StaticClass());
	PawnReference.Add(ATopDownPawn::StaticClass());

	index = 0;
	LastHitLocation = FVector{ 50,50,100 };

	//Assignment-3

	Message.BindUFunction(this, "NotifyMessage");
	IsDynamicMesh = true;
}

void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	Mapping = NewObject<UInputMappingContext>(this);


	OnLeftClickAction = NewObject<UInputAction>(this);
	OnLeftClickAction->ValueType = EInputActionValueType::Boolean;

	HideScrollBoxes = NewObject<UInputAction>(this);
	HideScrollBoxes->ValueType = EInputActionValueType::Boolean;
	

	if(Mapping)
	{
		Mapping->MapKey(OnLeftClickAction, EKeys::LeftMouseButton);
		Mapping->MapKey(HideScrollBoxes, EKeys::Tab);
	}

	if(EIC)
	{
		EIC->BindAction(OnLeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftClick);
		EIC->BindAction(HideScrollBoxes, ETriggerEvent::Completed, this, &AInteractiveArchController::HideVisibility);
	}

	//CameraPawnMapping
	CameraPawnMapping = NewObject<UInputMappingContext>(this);

	TogglePawn = NewObject<UInputAction>(this);
	TogglePawn->ValueType = EInputActionValueType::Boolean;

	if (CameraPawnMapping)
		CameraPawnMapping->MapKey(TogglePawn, EKeys::P);
	if (EIC)
	{
		EIC->BindAction(TogglePawn, ETriggerEvent::Completed, this, &AInteractiveArchController::Spawn);
	}

	//Spline Mapping

	MappingContext = NewObject<UInputMappingContext>(this);

	OnClick = NewObject<UInputAction>(this);
	OnClick->ValueType = EInputActionValueType::Boolean;

	CreateNewSpline = NewObject<UInputAction>(this);
	CreateNewSpline->ValueType = EInputActionValueType::Boolean;

	NextSpline = NewObject<UInputAction>(this);
	NextSpline->ValueType = EInputActionValueType::Boolean;

	LastSpline = NewObject<UInputAction>(this);
	LastSpline->ValueType = EInputActionValueType::Boolean;

	LatestSpline = NewObject<UInputAction>(this);
	LatestSpline->ValueType = EInputActionValueType::Boolean;

	Undo = NewObject<UInputAction>(this);
	Undo->ValueType = EInputActionValueType::Boolean;

	DestroyWall = NewObject<UInputAction>(this);
	DestroyWall->ValueType = EInputActionValueType::Boolean;

	check(EIC)
		EIC->BindAction(OnClick, ETriggerEvent::Completed, this, &AInteractiveArchController::GenerateWall);
	EIC->BindAction(CreateNewSpline, ETriggerEvent::Completed, this, &AInteractiveArchController::NewSpline);
	EIC->BindAction(NextSpline, ETriggerEvent::Completed, this, &AInteractiveArchController::ForwardSpline);
	EIC->BindAction(LastSpline, ETriggerEvent::Completed, this, &AInteractiveArchController::BackWardSpline);
	EIC->BindAction(LatestSpline, ETriggerEvent::Completed, this, &AInteractiveArchController::NewestSpline);
	EIC->BindAction(Undo, ETriggerEvent::Completed, this, &AInteractiveArchController::UndoWall);
	EIC->BindAction(DestroyWall, ETriggerEvent::Completed, this, &AInteractiveArchController::Destroy);

	if (MappingContext) {
		MappingContext->MapKey(OnClick, EKeys::LeftMouseButton);
		MappingContext->MapKey(CreateNewSpline, EKeys::RightMouseButton);
		MappingContext->MapKey(NextSpline, EKeys::Right);
		MappingContext->MapKey(LastSpline, EKeys::Left);
		MappingContext->MapKey(LatestSpline, EKeys::Up);
		MappingContext->MapKey(Undo, EKeys::Z);
		MappingContext->MapKey(DestroyWall, EKeys::B);
	}

	//Add Mapping Into Controller
	check(GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(SubSystem);
	SubSystem->AddMappingContext(Mapping, 0);
	SubSystem->AddMappingContext(CameraPawnMapping, 0);
}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();
	if(_SelectionWidget){
		SelectionWidget = CreateWidget<USelectionWidget>(this, _SelectionWidget);
	}
	if (_DisplayWidget) {
		DisplayWidget = CreateWidget<UDisplayMessage>(this, _DisplayWidget);
	}
	Spawn();
}

void AInteractiveArchController::OnLeftClick()
{
	FHitResult HitonClick;
	FVector WorldPosition, WorldDirection;
	check(DeprojectMousePositionToWorld(WorldPosition, WorldDirection))
		FVector TraceEnd;
	TraceEnd = WorldPosition + WorldDirection * 10000;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;

	if (GetWorld()->LineTraceSingleByChannel(HitonClick, WorldPosition, TraceEnd, ECC_Visibility, QueryParams)) {
		LastHitLocation = HitonClick.Location;
		SpawnedMeshActor = Cast<AMeshActor>(HitonClick.GetActor());
		if (SpawnedMeshActor) {
			LastHitLocation = HitonClick.GetActor()->GetActorLocation();
			if (GetPawn()->GetClass() == AIsometricCameraPawn::StaticClass())
			{
				GetPawn()->SetActorLocation(LastHitLocation + FVector(0, 0, 200));
			}
			else if (GetPawn()->GetClass() == ATopDownPawn::StaticClass()) {
				GetPawn()->SetActorLocation(LastHitLocation + FVector(-150, 0, 50));
			}
			else {
				GetPawn()->SetActorLocation(LastHitLocation);
			}
			if (SelectionWidget) {
				SelectionWidget->ScrollBox_1->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->ScrollBox_2->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->ScrollBox_3->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else {
			if (GetPawn()->GetClass() == AIsometricCameraPawn::StaticClass())
			{
				GetPawn()->SetActorLocation(LastHitLocation + FVector(0, 0, 200));
			}
			else if (GetPawn()->GetClass() == ATopDownPawn::StaticClass()) {
				GetPawn()->SetActorLocation(LastHitLocation + FVector(-150, 0, 50));
			}
			else {
				GetPawn()->SetActorLocation(LastHitLocation);
			}
			IsDestroyed = true;
			if(SelectionWidget){
				if(SelectionWidget->ScrollBox_1->ScrollBoxType == EScrollBoxType::MeshData){
					SelectionWidget->ScrollBox_1->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox_2->SetVisibility(ESlateVisibility::Hidden);
					SelectionWidget->ScrollBox_3->SetVisibility(ESlateVisibility::Hidden);
				}
				else if(SelectionWidget->ScrollBox_2->ScrollBoxType == EScrollBoxType::MeshData) {
					SelectionWidget->ScrollBox_1->SetVisibility(ESlateVisibility::Hidden);
					SelectionWidget->ScrollBox_2->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox_3->SetVisibility(ESlateVisibility::Hidden);
				}
				else if(SelectionWidget->ScrollBox_3->ScrollBoxType == EScrollBoxType::MeshData){
					SelectionWidget->ScrollBox_1->SetVisibility(ESlateVisibility::Hidden);
					SelectionWidget->ScrollBox_2->SetVisibility(ESlateVisibility::Hidden);
					SelectionWidget->ScrollBox_3->SetVisibility(ESlateVisibility::Visible);
				}

				SelectionWidget->ScrollBox_1->Controller.BindUFunction(this, "SpawnMesh");
				SelectionWidget->ScrollBox_1->ApplyMaterial.BindUFunction(this, "SetMaterial");
				SelectionWidget->ScrollBox_1->ApplyTexture.BindUFunction(this, "SetTexture");
				SelectionWidget->ScrollBox_2->Controller.BindUFunction(this, "SpawnMesh");
				SelectionWidget->ScrollBox_2->ApplyMaterial.BindUFunction(this, "SetMaterial");
				SelectionWidget->ScrollBox_2->ApplyTexture.BindUFunction(this, "SetTexture");
				SelectionWidget->ScrollBox_3->Controller.BindUFunction(this, "SpawnMesh");
				SelectionWidget->ScrollBox_3->ApplyMaterial.BindUFunction(this, "SetMaterial");
				SelectionWidget->ScrollBox_3->ApplyTexture.BindUFunction(this, "SetTexture");
			}
			if (SelectionWidget && !SelectionWidget->IsInViewport())
			{
				SelectionWidget->AddToViewport();
			}
		}
	}
}

void AInteractiveArchController::SpawnMesh(const FMeshData& MeshData)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>();
	MeshComponent->SetStaticMesh(MeshData.Mesh);
	FVector PivotLocation = MeshComponent->GetComponentLocation();
	MeshComponent->DestroyComponent();

	FRotator Rotation = FRotator::ZeroRotator;
	FBox BoundingBox = MeshData.Mesh->GetBoundingBox();
	FVector MinBounds = BoundingBox.Min;
	FVector MaxBounds = BoundingBox.Max;
	FVector center = BoundingBox.GetCenter();
	float OffsetZ = MaxBounds.Z;

	if(IsDestroyed){
		if(PivotLocation.Equals(center))
			SpawnedMeshActor = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass(), LastHitLocation + FVector(0, 0, OffsetZ), Rotation, Params);
		else
			SpawnedMeshActor = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass(), LastHitLocation, Rotation, Params);
		//GetPawn()->SetActorLocation(LastHitLocation);
		IsDestroyed = false;
	}
	else {
		if (SpawnedMeshActor) {
			SpawnedMeshActor->Destroy();
			if (PivotLocation.Equals(center))
				SpawnedMeshActor = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass(), LastHitLocation + FVector(0, 0, OffsetZ), Rotation, Params);
			else
				SpawnedMeshActor = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass(), LastHitLocation, Rotation, Params);
			//GetPawn()->SetActorLocation(LastHitLocation);
			IsDestroyed = false;
		}
	}
	if (SpawnedMeshActor) {
		SpawnedMeshActor->SetMobility(EComponentMobility::Movable);
		SpawnedMeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);
	}
}

void AInteractiveArchController::SetMaterial(const FMaterialData& MaterialData)
{
	if (SpawnedMeshActor && MaterialData.Material) {
		SpawnedMeshActor->GetStaticMeshComponent()->SetMaterial(0 , MaterialData.Material);
	}
}

void AInteractiveArchController::SetTexture(const FTextureData& TextureData)
{
	if (SpawnedMeshActor && TextureData.Texture) {
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(SpawnedMeshActor->GetStaticMeshComponent()->GetMaterial(0));
		if (!DynamicMaterial) 
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(SpawnedMeshActor->GetStaticMeshComponent()->GetMaterial(0), this);
		}
		DynamicMaterial->SetTextureParameterValue("Texture", TextureData.Texture);
		SpawnedMeshActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
	}
}

void AInteractiveArchController::HideVisibility()
{
	if (SelectionWidget->IsInViewport()) {

		ESlateVisibility SlateVisibility = SelectionWidget->GetVisibility();

		switch (SlateVisibility) {
		case ESlateVisibility::Visible:
			SlateVisibility = ESlateVisibility::Hidden;
			break;
		case ESlateVisibility::Hidden:
			SlateVisibility = ESlateVisibility::Visible;
			break;
		default:
			SlateVisibility = ESlateVisibility::Hidden;
			break;
		}
	SelectionWidget->SetVisibility(SlateVisibility);
	}
}

void AInteractiveArchController::Spawn()
{
	UWorld* World = GetWorld();

	if (!World) {
		return;
	}

	if (index == PawnReference.Num()) { index = 0; }
	TSubclassOf<APawn> CharacterClass = PawnReference[index]; // Replace with your character class 

	if (CharacterClass) {

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FRotator Rotation = FRotator::ZeroRotator;

		if (GetPawn()) {
			GetPawn()->Destroy();
		}

		check(GetLocalPlayer());
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		SubSystem->ClearAllMappings();

		if(PawnReference[index]){
			APawn* SpawnedCharacter;
			if (PawnReference[index] == AIsometricCameraPawn::StaticClass())
			{
				SpawnedCharacter = World->SpawnActor<APawn>(CharacterClass, LastHitLocation + FVector(0, 0, 200), Rotation, Params);
			}
			else if (PawnReference[index] == ATopDownPawn::StaticClass()) {
				SpawnedCharacter = World->SpawnActor<APawn>(CharacterClass, LastHitLocation + FVector(-150, 0, 50), Rotation, Params);
			}
			else {
				SpawnedCharacter = World->SpawnActor<APawn>(CharacterClass, LastHitLocation, Rotation, Params);
			}

			check(GetLocalPlayer());
			SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			check(SubSystem);
			SubSystem->AddMappingContext(Mapping, 0);
			SubSystem->AddMappingContext(CameraPawnMapping, 0);

			if (SpawnedCharacter) {
				Possess(SpawnedCharacter);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character"));
			}
		}
	}
	index++;
}


//Assignment-3

void AInteractiveArchController::GenerateWall()
{
	if (ArrayOfSplines.Num() != 0) {
		FHitResult HitonClick;
		GetHitResultUnderCursor(ECC_Visibility, true, HitonClick);

		if (HitonClick.bBlockingHit)
		{
			FVector ClickLocation = HitonClick.Location;
			ArrayOfSplines[SplineIndex]->Points.Add(HitonClick.Location);
			ArrayOfSplines[SplineIndex]->AddPoint(ClickLocation);

			if (ArrayOfSplines[SplineIndex]->Points.Num() >= 2) {
				FString Msg = "On Spline " + FString::FromInt(SplineIndex + 1) + " Wall " + FString::FromInt(ArrayOfSplines[SplineIndex]->Points.Num() - 1) + " Generated";
				Message.ExecuteIfBound(Msg);
			}
		}
	}
	else {
		FString Msg = "Right Click To Generate Spline";
		Message.ExecuteIfBound(Msg);
	}
}

void AInteractiveArchController::NewSpline()
{
	if (ArrayOfSplines.Num() > 0) {
		if (ArrayOfSplines[ArrayOfSplines.Num() - 1]->Points.Num() >= 2) {
			ASplineActor* Spline = NewObject<ASplineActor>(this);
			ArrayOfSplines.Add(Spline);
			SplineIndex = ArrayOfSplines.Num() - 1;

			FString Msg = "New Spline " + FString::FromInt(SplineIndex + 1) + " Generated";
			Message.ExecuteIfBound(Msg);
		}
		else {
			FString Msg = "Atleast Create A Wall Before Creating New Spline";
			Message.ExecuteIfBound(Msg);
		}
	}
	else {
		ASplineActor* Spline = NewObject<ASplineActor>(this);
		ArrayOfSplines.Add(Spline);
		SplineIndex = ArrayOfSplines.Num() - 1;

		FString Msg = "New Wall Spline " + FString::FromInt(SplineIndex + 1) + " Generated";
		Message.ExecuteIfBound(Msg);
	}
}

void AInteractiveArchController::BackWardSpline()
{
	if (SplineIndex == 0) {
		FString Msg = "Already On Last Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
	else {
		SplineIndex--;
		FString Msg = "Now Working On Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
}

void AInteractiveArchController::ForwardSpline()
{
	if (SplineIndex == ArrayOfSplines.Num() - 1) {
		FString Msg = "Already On Latest Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
	else {
		SplineIndex++;
		FString Msg = "Now Working On Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
}

void AInteractiveArchController::NewestSpline()
{
	if (SplineIndex == ArrayOfSplines.Num() - 1) {
		FString Msg = "Already On Latest Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
	else {
		SplineIndex = ArrayOfSplines.Num() - 1;
		FString Msg = "Now Working On Latest Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
}

void AInteractiveArchController::UndoWall()
{
	ArrayOfSplines[SplineIndex]->Undo();
	if (ArrayOfSplines[SplineIndex]->Points.Num() >= 1) {
		FString Msg = "Wall " + FString::FromInt(ArrayOfSplines[SplineIndex]->Points.Num() - 1) + " Of Spline " + FString::FromInt(SplineIndex + 1) + " Is Destroyed";
		Message.ExecuteIfBound(Msg);
	}
	else {
		FString Msg = "There is No Wall To Destroy";
		Message.ExecuteIfBound(Msg);
	}
}

void AInteractiveArchController::Destroy()
{
	ArrayOfSplines[SplineIndex]->DestroyAllWall();
	if (SplineIndex != (ArrayOfSplines.Num() - 1))
		ArrayOfSplines.RemoveAt(SplineIndex);
	FString Msg = "Spline " + FString::FromInt(SplineIndex + 1) + " Is Destroyed";
	Message.ExecuteIfBound(Msg);
}

void AInteractiveArchController::SwitchController()
{
	if (IsDynamicMesh) {
		SelectionWidget->RemoveFromViewport();
		DisplayWidget->AddToViewport(0);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(MappingContext, 0);
		SubSystem->AddMappingContext(CameraPawnMapping, 0);
		IsDynamicMesh = false;
	}
	else {
		DisplayWidget->RemoveFromViewport();
		SelectionWidget->AddToViewport(0);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(Mapping, 0);
		SubSystem->AddMappingContext(CameraPawnMapping, 0);
		IsDynamicMesh = true;
	}
}
