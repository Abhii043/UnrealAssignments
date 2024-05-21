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
#include "TopDownPawn.h"

#include "SplineActor.h"
#include "Delegates/Delegate.h" 
#include "DisplayMessage.h"

#include "InteractiveArchController.generated.h"

DECLARE_DELEGATE_OneParam(GenerateMsg, FString)

UCLASS()
class ASSIGNMENT_3_4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
	
	AInteractiveArchController();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchController")
	UInputMappingContext* Mapping;

	UPROPERTY(BlueprintReadOnly , Category = "InteractiveArchController" )
	UInputAction* OnLeftClickAction;

	UPROPERTY(BlueprintReadOnly , Category = "InteractiveArchController" )
	UInputAction* HideScrollBoxes;

	UPROPERTY(BlueprintReadWrite, Category = "InteractiveArchController")
	FVector LastHitLocation ;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "InteractiveArchController")
	TSubclassOf<USelectionWidget> _SelectionWidget; 

	UPROPERTY(BlueprintReadWrite, Category = "InteractiveArchController")
	USelectionWidget* SelectionWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "InteractiveArchController")
	bool IsDestroyed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InteractiveArchController")
	AMeshActor* SpawnedMeshActor;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchController")
	TArray<TSubclassOf<APawn>> PawnReference ;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchController")
	int32 index;

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void Spawn();

	UFUNCTION(BlueprintCallable , Category = "InteractiveArchController")
	void OnLeftClick();	

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void SpawnMesh(const FMeshData& MeshData);

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void SetMaterial(const FMaterialData& MaterialData);

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void SetTexture(const FTextureData& MaterialData);

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchController")
	void HideVisibility(); 


	//Assignment-3

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* OnClick;
	
	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* Undo;
	
	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* DestroyWall;
	
	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* CreateNewSpline;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* NextSpline;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* LastSpline;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputAction* LatestSpline;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	UInputMappingContext* MappingContext;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	TArray<ASplineActor*> ArrayOfSplines;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSplineController")
	int32 SplineIndex;

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void GenerateWall();
	
	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void NewSpline();	

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void BackWardSpline();	

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void ForwardSpline();
	
	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void NewestSpline();

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void UndoWall();
	
	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSplineController")
	void Destroy();

	//Delegate And its Function

	GenerateMsg Message;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyMessage(const FString& msg);

	//For switching Controll Between Assignments

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "InteractiveArchSplineController")
	TSubclassOf<UDisplayMessage> _DisplayWidget; 

	UPROPERTY(BlueprintReadWrite, Category = "InteractiveArchSplineController")
	UDisplayMessage* DisplayWidget;

	UFUNCTION(BlueprintCallable, Category = "InteractiveArchSwitchController")
	void SwitchController();

	UPROPERTY(BlueprintReadOnly , Category = "InteractiveArchSwitchController")
	bool IsDynamicMesh;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSwitchController")
	UInputAction* TogglePawn;

	UPROPERTY(BlueprintReadOnly, Category = "InteractiveArchSwitchController")
	UInputMappingContext* CameraPawnMapping;
};
