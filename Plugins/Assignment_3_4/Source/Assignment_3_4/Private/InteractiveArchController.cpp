// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

AInteractiveArchController::AInteractiveArchController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	IsDestroyed = true;

	OrthoPawn = CreateDefaultSubobject<AOrthographicCameraPawn>(TEXT("OrthoPawn"));
	PerspectivePawn = CreateDefaultSubobject<APespectiveCameraPawn>(TEXT("PerspectivePawn"));
	IsometricPawn = CreateDefaultSubobject<AIsometricCameraPawn>(TEXT("IsometricPawn"));

	PawnReference.Add(OrthoPawn->GetClass());
	PawnReference.Add(PerspectivePawn->GetClass());
	PawnReference.Add(IsometricPawn->GetClass());

	index = 0;
	LastHitLocation = FVector{ 50,50,100 };
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
	
	TogglePawn = NewObject<UInputAction>(this);
	TogglePawn->ValueType = EInputActionValueType::Boolean;

	check(Mapping)
		Mapping->MapKey(OnLeftClickAction, EKeys::LeftMouseButton);
		Mapping->MapKey(HideScrollBoxes, EKeys::Tab);
		Mapping->MapKey(TogglePawn, EKeys::P);

	check(EIC)
		EIC->BindAction(OnLeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftClick);
		EIC->BindAction(HideScrollBoxes, ETriggerEvent::Completed, this, &AInteractiveArchController::HideVisibility);
		EIC->BindAction(TogglePawn, ETriggerEvent::Completed, this, &AInteractiveArchController::Spawn);

	check(GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(SubSystem);
	SubSystem->AddMappingContext(Mapping, 0);

}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();
	if(_SelectionWidget){
		SelectionWidget = CreateWidget<USelectionWidget>(this, _SelectionWidget);
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
			if (GetPawn()->GetClass() == IsometricPawn->GetClass())
			{
				GetPawn()->SetActorLocation(LastHitLocation + FVector(0, 0, 200));
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
			if (GetPawn()->GetClass() == IsometricPawn->GetClass())
			{
				GetPawn()->SetActorLocation(LastHitLocation + FVector(0, 0, 200));
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

		APawn* SpawnedCharacter;
		if(PawnReference[index] == IsometricPawn->GetClass())
			{
			SpawnedCharacter = World->SpawnActor<APawn>(CharacterClass, LastHitLocation + FVector(0,0,200), Rotation, Params);
		}
		else {
			SpawnedCharacter = World->SpawnActor<APawn>(CharacterClass, LastHitLocation, Rotation, Params);
		}

		check(GetLocalPlayer());
		SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		SubSystem->AddMappingContext(Mapping, 0);

		if (SpawnedCharacter) {
			Possess(SpawnedCharacter);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character"));
		}
	}
	index++;
}
