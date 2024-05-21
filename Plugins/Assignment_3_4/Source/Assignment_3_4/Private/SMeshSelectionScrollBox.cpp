// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include <Brushes/SlateColorBrush.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	RootBorder = SNew(SBorder);
	FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.12549f, 0.043137f, 0.101961f, 1.0f));
	ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
	OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ColorBrush->OutlineSettings = OutlineSettings;
	RootBorder->SetBorderImage(ColorBrush);

	RootVerticalBox = SNew(SVerticalBox);
	RootBoxName = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FColor::White);
	ScrollBox = SNew(SScrollBox).Orientation(Orient_Horizontal);

	MeshDataAssetManager = InArgs._InMeshAssetManager;
	ScrollBoxType = InArgs._InScrollBoxType;
	ThumbnailSize = InArgs._InThumbnailSize;

	ScrollBoxSelection();

	RootVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
		[
			RootBoxName.ToSharedRef()
		];
	RootVerticalBox->AddSlot().HAlign(HAlign_Center)
		[
			ScrollBox.ToSharedRef()
		];

	RootBorder->SetContent(RootVerticalBox.ToSharedRef());

	ChildSlot
	[
		RootBorder.ToSharedRef()
	];
}

void SMeshSelectionScrollBox::ScrollBoxSelection()
{
	if(ScrollBoxType == EScrollBoxType::MeshData){
		RefreshAssetMesh();
	}
	else if (ScrollBoxType == EScrollBoxType::Material) {
		RefreshAssetMeshMaterial();
	}
	else if(ScrollBoxType == EScrollBoxType::Texture){
		RefreshAssetMeshTexture();
	}
}

void SMeshSelectionScrollBox::RefreshAssetMesh()
{	
	RootBoxName->SetText(FText::FromString("Mesh"));
	ScrollBox->ClearChildren();
	if (MeshDataAssetManager.IsValid()) {
		for (const FMeshData& MeshData : MeshDataAssetManager->MeshDataArray) {
			if (MeshData.Thumbnail){
				TSharedPtr<STextBlock> MeshName = SNew(STextBlock).Text(MeshData.MeshName).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(MeshData.Thumbnail);
				SlateBrush->ImageSize = FVector2D(ThumbnailSize);
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &MeshData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
						{
							if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
								OnMeshSelection.ExecuteIfBound(MeshData);
								return FReply::Handled();
							}
							return FReply::Unhandled();
						});
				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1,1,1,1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						MeshName.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];
			}
		}
	}
	
}

void SMeshSelectionScrollBox::RefreshAssetMeshMaterial()
{
	RootBoxName->SetText(FText::FromString("Material"));
	ScrollBox->ClearChildren();
	if (MeshDataAssetManager.IsValid()) {
		for (FMaterialData& MaterialData : MeshDataAssetManager->MaterialDataArray) {
			if (MaterialData.Thumbnail) {
				TSharedPtr<STextBlock> MaterialName = SNew(STextBlock).Text(MaterialData.MaterialName).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(MaterialData.Thumbnail);
				SlateBrush->ImageSize = FVector2D(ThumbnailSize);
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand).
					OnMouseButtonDown_Lambda(
						[this , &MaterialData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
						{
							if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
								OnMaterialSelection.ExecuteIfBound(MaterialData);
								return FReply::Handled();
							}
								return FReply::Unhandled();
						}
				);

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(15, 15, 15, 15));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						MaterialName.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];
			}
		}
	}
}

void SMeshSelectionScrollBox::RefreshAssetMeshTexture()
{
	RootBoxName->SetText(FText::FromString("Texture"));
	ScrollBox->ClearChildren();
	if (MeshDataAssetManager.IsValid()) {
		for (FTextureData& TextureData : MeshDataAssetManager->TextureDataArray) {
			if (TextureData.Texture) {
				TSharedPtr<STextBlock> TextureName = SNew(STextBlock).Text(TextureData.TextureName).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(TextureData.Texture);
				SlateBrush->ImageSize = FVector2D(ThumbnailSize);
				TSharedPtr<SImage> Texture = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand).
					OnMouseButtonDown_Lambda(
						[this, &TextureData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
						{
							if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
								OnTextureSelection.ExecuteIfBound(TextureData);
								return FReply::Handled();
							}
							return FReply::Unhandled();
						}
				);

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(15, 15, 15, 15));

				ScrollBoxBorder->SetContent(Texture.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						TextureName.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];
			}
		}
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION