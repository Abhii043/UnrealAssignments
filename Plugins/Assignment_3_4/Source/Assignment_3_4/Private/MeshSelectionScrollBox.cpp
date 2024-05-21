// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	ScrollBoxAsset = SNew(SMeshSelectionScrollBox).InMeshAssetManager(MeshDataAsset).InScrollBoxType(ScrollBoxType).InThumbnailSize(ThumbnailSize);
	ScrollBoxAsset->OnMeshSelection.BindUFunction(this, "HandleSelectedMesh");
	ScrollBoxAsset->OnMaterialSelection.BindUFunction(this, "HandleSelectedMaterial");
	ScrollBoxAsset->OnTextureSelection.BindUFunction(this, "HandleSelectedTexture"); 
	return ScrollBoxAsset.ToSharedRef();
}

void UMeshSelectionScrollBox::HandleSelectedMesh(const FMeshData& MeshData)
{
	Controller.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleSelectedMaterial(const FMaterialData& MaterialData)
{
	ApplyMaterial.ExecuteIfBound(MaterialData);
}

void UMeshSelectionScrollBox::HandleSelectedTexture(const FTextureData& TextureData)
{
	ApplyTexture.ExecuteIfBound(TextureData);
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	ScrollBoxAsset.Reset();
}