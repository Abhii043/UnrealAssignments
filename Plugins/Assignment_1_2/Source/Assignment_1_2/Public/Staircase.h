// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>
#include "Staircase.generated.h"

UENUM(BlueprintType, category = "Manipulate Staircase")
enum class EtypeOfStairs : uint8 {
	openTypeStairs UMETA(DisplayName = "OpenStairs"),
	closeTypeStairs UMETA(DisplayName = "CloseStairs"),
	boxTypeStairs UMETA(DisplayName = "BoxStairs")
};

USTRUCT(BlueprintType)
struct FStair {
	GENERATED_BODY()

	UPROPERTY()
	UStaticMeshComponent* Staircase{};

	UPROPERTY()
	UStaticMeshComponent* LeftRailing{};

	UPROPERTY()
	UStaticMeshComponent* RightRailing{};

	UPROPERTY()
	UStaticMeshComponent* LeftHandRailing{};

	UPROPERTY()
	UStaticMeshComponent* RightHandRailing{};

	void destroy() {
		if (Staircase)
		{
			Staircase->DestroyComponent();
			Staircase = nullptr;
		}
		if (LeftRailing) 
		{ 
			LeftRailing->DestroyComponent(); 
			Staircase = nullptr;
		}
		if (RightRailing)
		{
			RightRailing->DestroyComponent();
			RightRailing = nullptr;
		}
		if (RightHandRailing)
		{
			RightHandRailing->DestroyComponent();
			RightHandRailing = nullptr;
		}
		if (LeftHandRailing)
		{
			LeftHandRailing->DestroyComponent();
			LeftHandRailing = nullptr;
		}
	}
};

UCLASS()
class ASSIGNMENT_1_2_API AStaircase : public AActor
{
	GENERATED_BODY()

public:
	AStaircase();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase")
	TEnumAsByte<EtypeOfStairs> typeOfStairs;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase")
	UStaticMesh* staircaseMesh;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase", meta = (ClampMin = 1, UIMIN = 1))
	int numberOfStairs;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase", meta = (ClampMin = 0, UIMIN = 0))
	FVector dimensions;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase", meta = (ClampMin = 1.01, UIMIN = 1.01), meta = (EditCondition = "typeOfStairs == EtypeOfStairs::openTypeStairs"))
	float VerticalSpacingInOpenStairs;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase", meta = (ClampMin = 1.01, UIMIN = 1.01), meta = (EditCondition = "typeOfStairs == EtypeOfStairs::openTypeStairs"))
	float HorizontalSpacingInOpenStairs;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase")
	bool railing;

	UPROPERTY(VisibleAnywhere, category = "Manipulate Staircase")
	USceneComponent* scene;

	UPROPERTY()
	FVector leftRailingDimensions;

	UPROPERTY()
	FVector rightRailingDimensions;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase", meta = (EditCondition = "railing"))
	UStaticMesh* railingMesh;

	UPROPERTY()
	TArray<FStair> StaircaseComponents;

	UPROPERTY(EditAnywhere, category = "Manipulate Staircase", meta = (EditCondition = "railing"))
	UStaticMesh* railingHandMesh;

	UPROPERTY()
	FVector leftRailingHandMeshDimensions;

	UPROPERTY()
	FVector rightRailingHandMeshDimensions;

	void GenerateStairs();

};
