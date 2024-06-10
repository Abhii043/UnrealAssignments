// Fill out your copyright notice in the Description page of Project Settings.


#include "FAsyncMeshGeneratorTask.h"
#include "MeshGenerator.h"

FAsyncMeshGeneratorTask::FAsyncMeshGeneratorTask(AMeshGenerator* InMeshGenerator)
{
	MeshGenerator = InMeshGenerator;
}

void FAsyncMeshGeneratorTask::DoWork()
{
	if (!MeshGenerator || !IsValid(MeshGenerator)) { return; };
	if (MeshGenerator)
	{
		if (UMeshDataAsset* MeshDataAsset = MeshGenerator->MeshDataAsset)
		{
			TArray<FMeshProperties> MeshPropertiesArr = MeshDataAsset->MeshPropertiesArr;

			for (int iIndex = 0; iIndex < MeshGenerator->NumberOfInstances; iIndex++)
			{
				int RandomIndex = FMath::RandRange(0, MeshPropertiesArr.Num() - 1);

				UStaticMesh* CurrentMesh = MeshPropertiesArr[RandomIndex].StaticMesh;
				UMaterialInterface* Material = MeshPropertiesArr[RandomIndex].Material;

				float MinScale = MeshPropertiesArr[RandomIndex].MinScale;
				float MaxScale = MeshPropertiesArr[RandomIndex].MaxScale;
				float MinRotation = MeshPropertiesArr[RandomIndex].MinRotation;
				float MaxRotation = MeshPropertiesArr[RandomIndex].MaxRotation;

				TArray<FTransform> InstanceTransforms;
				FTransform Transform;

				if(MeshGenerator->SelectedBoundingBox == "Box") {
					FVector BoundingExtent = (MeshGenerator->SelectedActor->GetActorScale3D() * MeshGenerator->Dimensions) / 2;
					FVector Origin = MeshGenerator->SelectedActor->GetActorLocation();
					FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);
					FVector Position = FMath::RandPointInBox(BoundingBox);
					Transform.SetLocation(Position);
				}
				else {
					if(MeshGenerator && MeshGenerator->SelectedActor){
						float Radius = MeshGenerator->SelectedActor->GetActorScale3D().X * MeshGenerator->Dimensions.X;
						FVector Origin = MeshGenerator->SelectedActor->GetActorLocation();

						Transform.SetLocation(FMath::VRand() * FMath::FRandRange(0.0f, Radius) + Origin);
					}
				}

				Transform.SetScale3D(FVector(FMath::RandRange(MinScale, MaxScale)));
				Transform.SetRotation(FQuat(FRotator(FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation))));
				InstanceTransforms.Add(Transform);


				AsyncTask(ENamedThreads::GameThread, [this, CurrentMesh, InstanceTransforms, Material]()
					{
				MeshGenerator->AddInstances(CurrentMesh, InstanceTransforms , Material);

					});

				FPlatformProcess::Sleep(0.01f);
			}
		}
	}
}