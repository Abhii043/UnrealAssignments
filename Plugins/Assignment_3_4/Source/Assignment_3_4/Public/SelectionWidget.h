// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <MeshSelectionScrollBox.h>
#include "SelectionWidget.generated.h"


UCLASS()
class ASSIGNMENT_3_4_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(Meta = (BindWidget))
	UMeshSelectionScrollBox* ScrollBox_1;
	UPROPERTY(Meta = (BindWidget))
	UMeshSelectionScrollBox* ScrollBox_2;
	UPROPERTY(Meta = (BindWidget))
	UMeshSelectionScrollBox* ScrollBox_3;
};
