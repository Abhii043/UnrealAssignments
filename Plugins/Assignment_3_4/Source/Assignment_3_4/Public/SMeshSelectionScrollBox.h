// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAssetManager.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnThumbnailSelect , const FMeshData& )
DECLARE_DELEGATE_OneParam(FOnMaterialSelection , const FMaterialData& )
DECLARE_DELEGATE_OneParam(FOnTextureSelection , const FTextureData& )

UENUM()
enum class EScrollBoxType {
	MeshData ,
	Material ,
	Texture
};

class ASSIGNMENT_3_4_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshAssetManager)
	SLATE_ARGUMENT(EScrollBoxType, InScrollBoxType)
	SLATE_ARGUMENT(float, InThumbnailSize)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FOnThumbnailSelect OnMeshSelection;
	FOnMaterialSelection OnMaterialSelection;
	FOnTextureSelection OnTextureSelection;  

	TSharedPtr<SBorder> RootBorder;
	TSharedPtr<STextBlock> RootBoxName;
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> RootVerticalBox;
private:
	TWeakObjectPtr<UMeshAssetManager> MeshDataAssetManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SlateScrollBox")
	EScrollBoxType ScrollBoxType;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "SlateScrollBox" , Meta = (UIMin = 1.0f , ClampMin = 1.0f))
	float ThumbnailSize{1.0f};

	void ScrollBoxSelection();
	void RefreshAssetMesh();
	void RefreshAssetMeshMaterial();
	void RefreshAssetMeshTexture();

};
