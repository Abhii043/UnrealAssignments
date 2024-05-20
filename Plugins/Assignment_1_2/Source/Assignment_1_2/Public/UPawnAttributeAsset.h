// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UPawnAttributeAsset.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_1_2_API UUPawnAttributeAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere ,BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Meta = (UiMin = 0 , UiMax=100 , ClampMin = 0 , ClampMax = 100))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Meta = (UiMin = 0, UiMax = 10, ClampMin = 0, ClampMax = 10))
	float Jump;
};
