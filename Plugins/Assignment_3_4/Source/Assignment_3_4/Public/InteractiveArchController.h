// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MeshAssetManager.h"
#include "MeshSelectionScrollBox.h"
#include "Engine/StaticMeshActor.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <SelectionWidget.h>
#include "MeshActor.h"

#include "IsometricCameraPawn.h"
#include "OrthographicCameraPawn.h"
#include "PespectiveCameraPawn.h"
#include "InteractiveArchController.generated.h"

UCLASS()
class ASSIGNMENT_3_4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
	
	AInteractiveArchController();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InteractiveArchController")
	UInputMappingContext* Mapping;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "InteractiveArchController" )
	UInputAction* OnLeftClickAction;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "InteractiveArchController" )
	UInputAction* HideScrollBoxes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InteractiveArchController")
	FVector LastHitLocation ;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "InteractiveArchController")
	TSubclassOf<USelectionWidget> _SelectionWidget; 

	UPROPERTY(BlueprintReadWrite, Category = "InteractiveArchController")
	USelectionWidget* SelectionWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "InteractiveArchController")
	bool IsDestroyed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InteractiveArchController")
	AMeshActor* SpawnedMeshActor;

	UPROPERTY()
	TArray<TSubclassOf<APawn>> PawnReference ;
	UPROPERTY()
	AOrthographicCameraPawn* OrthoPawn;
	UPROPERTY()
	APespectiveCameraPawn* PerspectivePawn;
	UPROPERTY()
	AIsometricCameraPawn* IsometricPawn;
	UPROPERTY()
	int32 index;
	UPROPERTY()
	UInputAction* TogglePawn;
	UFUNCTION(BlueprintCallable)
	void Spawn();

	UFUNCTION(BlueprintCallable , Category = "InteractiveArchController")
	void OnLeftClick();	

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void SpawnMesh(const FMeshData& MeshData);

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void SetMaterial(const FMaterialData& MaterialData);

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void SetTexture(const FTextureData& MaterialData);

	UFUNCTION(BlueprintCallable)
	void HideVisibility(); 
};
