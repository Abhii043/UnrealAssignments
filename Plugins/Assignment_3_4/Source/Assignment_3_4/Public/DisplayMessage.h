// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "DisplayMessage.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_3_4_API UDisplayMessage : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite ,Meta = (BindWidget))
	UTextBlock* DisplayMessage;
};
