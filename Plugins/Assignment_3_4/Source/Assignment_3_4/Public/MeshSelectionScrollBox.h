// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshAssetManager.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FOnCotroller, const FMeshData& MeshData);
DECLARE_DELEGATE_OneParam(FApplyMaterial, const FMaterialData& MaterialData);
DECLARE_DELEGATE_OneParam(FApplyTexture, const FTextureData& TextureData);

UCLASS()
class ASSIGNMENT_3_4_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere ,BlueprintReadWrite , Category = "MeshData")
	UMeshAssetManager* MeshDataAsset;

	FOnCotroller Controller;

	FApplyMaterial ApplyMaterial; 
	
	FApplyTexture ApplyTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollBox" , meta = (AllowPrivateAccess = "true"))
	EScrollBoxType ScrollBoxType;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<SMeshSelectionScrollBox> ScrollBoxAsset;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "ScrollBox" , Meta = (UIMin = 1.0f , ClampMin = 1.0f , AllowPrivateAccess = "true"))
	float ThumbnailSize{1.0f};

	UFUNCTION()
	void HandleSelectedMesh(const FMeshData& MeshData);

	UFUNCTION()
	void HandleSelectedMaterial(const FMaterialData& MaterialData);

	UFUNCTION()
	void HandleSelectedTexture(const FTextureData& TextureData);

	void ReleaseSlateResources(bool bReleaseChildren);

	virtual const FText GetPaletteCategory() override;
	
};
