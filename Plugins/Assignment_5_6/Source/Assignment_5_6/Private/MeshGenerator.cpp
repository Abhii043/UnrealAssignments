// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

// Sets default values
AMeshGenerator::AMeshGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeshGenerator::ScatterObjects()
{
	Progress = 1;
	for (auto& Pair : HISMComponents) {
		if (UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value)
		{
			HISM->ClearInstances();
		}
	}

	if (AsyncMeshGeneratorTask)
	{
		if (!AsyncMeshGeneratorTask->IsDone()) {
			AsyncMeshGeneratorTask->EnsureCompletion();

		}
		delete AsyncMeshGeneratorTask;
		AsyncMeshGeneratorTask = nullptr;
	}

	AsyncMeshGeneratorTask = new FAsyncTask<FAsyncMeshGeneratorTask>(this);
	AsyncMeshGeneratorTask->StartBackgroundTask();
}

void AMeshGenerator::AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms , UMaterialInterface* Material)
{
	if (!this || !IsValid(this)) { return; };
	UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
	if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
	{
		(*HISMCPtr)->AddInstances(Transforms, false);
	}
	else
	{
		UHierarchicalInstancedStaticMeshComponent* NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
		NewHISMC->SetStaticMesh(StaticMesh);
		NewHISMC->SetMaterial(0, Material);

		HISMComponents.Add(StaticMesh, NewHISMC);

		NewHISMC->RegisterComponentWithWorld(GetWorld());
		NewHISMC->AddInstances(Transforms, false);
	}

	float PercentOfProgress = Progress / float(NumberOfInstances);

	UpdateProgessBar(PercentOfProgress);
	Progress++;

}