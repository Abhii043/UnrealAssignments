// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

USTRUCT(BlueprintType)
struct FMeshData {

	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Meshdata")
	FText MeshName;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Meshdata")
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Meshdata")
	UTexture2D* Thumbnail;
};
 
USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FText MaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* Thumbnail;
};
 
USTRUCT(BlueprintType)
struct FTextureData {
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	FText TextureName;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* Texture;
};

UCLASS()
class ASSIGNMENT_3_4_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Asset")
	TArray<FMeshData> MeshDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
	TArray<FMaterialData> MaterialDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
	TArray<FTextureData> TextureDataArray;
};
