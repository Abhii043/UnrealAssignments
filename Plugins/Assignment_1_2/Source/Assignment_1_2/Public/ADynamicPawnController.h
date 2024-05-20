// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h" 
#include "InputMappingContext.h"
#include "InputAction.h"
#include "ADynamicPawnController.generated.h"

UENUM()
enum class EPawn {
	ThirdPersonPawn,
	TopDownPawn ,
	FPP
};

USTRUCT(BlueprintType)
struct FPawnClass : public FTableRowBase{  

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPawn Charachter{};

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> ClassReference{}; 

	FPawnClass() {}
};

UCLASS()
class ASSIGNMENT_1_2_API AADynamicPawnController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AADynamicPawnController();

	virtual void BeginPlay() override;

	void Spawn();
	 
	TSubclassOf<APawn> CharacterPawn();

	virtual void SetupInputComponent() override;

	UPROPERTY()
	UDataTable* DataTable;

	UPROPERTY()
	int index; 

	UPROPERTY()
	UInputMappingContext* InputmappingContext;

	UPROPERTY()
	UInputAction* ToggleAction;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UUPawnAttributeAsset* Asset);

	FPawnClass* PawnDataTableRow;
};
