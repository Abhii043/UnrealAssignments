// Fill out your copyright notice in the Description page of Project Settings.


#include "ADynamicPawnController.h"
#include "GameFramework/Character.h"
#include "Engine/AssetManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AFirstPersonPawn.h"  
#include "ABaseCharacter.h"   

AADynamicPawnController::AADynamicPawnController() : index{0} {

	DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Assignment_1_2/Assignment_2/DataTable/PawnClassSelector.PawnClassSelector'"));
}

void AADynamicPawnController::BeginPlay()
{
	Super::BeginPlay();
	//Spawn(); 
}

void AADynamicPawnController::Spawn()
{
	UWorld* World = GetWorld();

	if(!World) {
		return;
	}

	TSubclassOf<APawn> CharacterClass = CharacterPawn(); // Replace with your character class  

	if(CharacterClass){

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location; 
		FRotator Rotation; 

		if (GetPawn()) {
			Location = GetPawn()->GetActorLocation(); 
			Rotation = GetPawn()->GetActorRotation(); 

			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) { 
				Subsystem->ClearAllMappings(); 
			} 

			GetPawn()->Destroy(); 
		}
		else {
			Location = FVector{ 50,50,100 }; 
			Rotation = FRotator{ 0,0,0 };; 
		}

		APawn* SpawnedCharacter = World->SpawnActor<APawn>(CharacterClass, Location, Rotation, Params); 

		if (SpawnedCharacter) {
			Possess(SpawnedCharacter);
			
			if(UEnhancedInputLocalPlayerSubsystem * Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(InputmappingContext, 0);
			}

			if (PawnDataTableRow->Charachter == EPawn::FPP) {
				if (Cast<AAFirstPersonPawn>(SpawnedCharacter)) {
					DisplayAttributes(Cast<AAFirstPersonPawn>(SpawnedCharacter)->DataAsset);
				}
			}
			else {
				if (Cast<AABaseCharacter>(SpawnedCharacter)) {
					DisplayAttributes(Cast<AABaseCharacter>(SpawnedCharacter)->DataAsset);
				}
			}

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character"));
		}
	}
}

TSubclassOf<APawn> AADynamicPawnController::CharacterPawn() 
{
	if (DataTable) { 
		TArray<FName> RowNames = DataTable->GetRowNames(); 

		if (index == RowNames.Num()) {
			index = 0;
		}

		PawnDataTableRow = DataTable->FindRow<FPawnClass>(RowNames[index++], TEXT(""));  

		if (PawnDataTableRow->Charachter == EPawn::TopDownPawn) { 
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;
		} 
		else {
			bShowMouseCursor = false;
			bEnableClickEvents = false;
			bEnableMouseOverEvents = false;
		}
		if (PawnDataTableRow) {
			return PawnDataTableRow->ClassReference;
		}		
	}

	return nullptr;
}

void AADynamicPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputmappingContext = NewObject<UInputMappingContext>(this); 

	ToggleAction = NewObject<UInputAction>(this); 

	ToggleAction->ValueType = EInputActionValueType::Boolean; 

	InputmappingContext->MapKey(ToggleAction, EKeys::P); 

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent); 

	if (EnhancedInput) { 
		EnhancedInput->BindAction(ToggleAction, ETriggerEvent::Completed, this, &AADynamicPawnController::Spawn); 
	} 
	 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem); 

	Subsystem->AddMappingContext(InputmappingContext, 0); 
}
