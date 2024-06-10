// Fill out your copyright notice in the Description page of Project Settings.


#include "FAsyncMeshGeneratorTask.h"
#include "MeshGenerator.h"
#include "Async/ParallelFor.h"

FAsyncMeshGeneratorTask::FAsyncMeshGeneratorTask(AMeshGenerator* InMeshGenerator)
{
	MeshGenerator = InMeshGenerator;
}

void FAsyncMeshGeneratorTask::DoWork()
{
	if (MeshGenerator)
	{
		if (UMeshDataAsset* MeshDataAsset = MeshGenerator->MeshDataAsset)
		{
			TArray<FMeshProperties> MeshPropertiesArr = MeshDataAsset->MeshPropertiesArr;

			for (int Index = 0; Index < MeshPropertiesArr.Num(); Index++)
			{
				//int32 RandomMeshIndex = FMath::RandRange(0, StaticMeshes.Num() - 1);
				UStaticMesh* CurrentMesh = MeshPropertiesArr[Index].StaticMesh;

				for (int iIndex = 0; iIndex < MeshGenerator->NumberOfInstances/3; iIndex++)
				{
					FVector BoundingExtent = (MeshGenerator->SelectedActor->GetActorScale3D() * MeshGenerator->Dimensions)/2;
					FVector Origin = MeshGenerator->SelectedActor->GetActorLocation();

					FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

					FVector Position = FMath::RandPointInBox(BoundingBox);

					TArray<FTransform> InstanceTransforms;
					InstanceTransforms.Add(FTransform(Position));
					MeshGenerator->AddInstances(CurrentMesh, InstanceTransforms);
					FPlatformProcess::Sleep(0.01);
				}
			}
		}
	}
}
