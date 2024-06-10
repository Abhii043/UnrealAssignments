// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMeshProperties {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MinScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MaxScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MinRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MaxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties" , Meta = (ClampMin = 1 , UIMin = 1 , UIMax = 10 , ClampMax = 10))
	float footPrint;
};

UCLASS()
class ASSIGNMENT_5_6_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	TArray<FMeshProperties> MeshPropertiesArr;
};
