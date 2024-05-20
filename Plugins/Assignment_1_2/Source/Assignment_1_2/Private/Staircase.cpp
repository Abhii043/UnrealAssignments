// Fill out your copyright notice in the Description page of Project Settings.

#include "Staircase.h"

// Fill out your copyright notice in the Description page of Project Settings.

AStaircase::AStaircase() : numberOfStairs{ true }, dimensions{ FVector(1.1f, 4.0f, 0.3f) }, leftRailingDimensions{ FVector(0.15f, 0.025f, 5.0f) }, rightRailingDimensions{ FVector(0.15f, 0.025f, 5.0f) }, VerticalSpacingInOpenStairs{ 1.2 }, HorizontalSpacingInOpenStairs{ 1 }, railing{ false }, leftRailingHandMeshDimensions { 1.0f, 0.025f, 0.5f } , rightRailingHandMeshDimensions{ 1.0f, 0.025f, 0.5f }
{
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("Staircase on Constructed"));

	//Construction Of SceneRoot Of Staircase Actor
	scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootOfActor"));
	RootComponent = scene;

}

void AStaircase::OnConstruction(const FTransform& Transform)
{
	GenerateStairs();
}

void AStaircase::BeginPlay()
{
}

void AStaircase::Tick(float DeltaTime)
{
}

void AStaircase::GenerateStairs()
{
	//For Destroying Stairs And Railings
	for (int index = 0; index < StaircaseComponents.Num(); index++) {
		StaircaseComponents[index].destroy();
	}
	StaircaseComponents.Empty();

	//For Creating Staircase With Railings Or Without It
	int i;
	FStair Stair{};
	int sizeOfBoxStairs = numberOfStairs;
	FVector DimentionsOfStairMesh{ 1,1,1 };
	FVector DimentionsOfRailingMesh{ 1,1,1 };
	for (i = 0; i < numberOfStairs; i++) {
		UE_LOG(LogTemp, Warning, TEXT("Staircase on OnConstruction %d"), i);
		FString staircaseMeshName = "StairCaseMeshComponent" + FString::FromInt(i);
		UStaticMeshComponent* staircaseMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *staircaseMeshName);
		if (staircaseMeshComponent) {

			staircaseMeshComponent->SetupAttachment(scene);
			staircaseMeshComponent->SetStaticMesh(staircaseMesh);

			if (staircaseMesh) {
				DimentionsOfStairMesh = staircaseMesh->GetBounds().GetBox().GetSize();

				if (typeOfStairs == EtypeOfStairs::openTypeStairs) {
					staircaseMeshComponent->SetRelativeScale3D(FVector((dimensions.X), dimensions.Y, dimensions.Z));
					staircaseMeshComponent->SetRelativeLocation(FVector(((DimentionsOfStairMesh.X * dimensions.X * i * HorizontalSpacingInOpenStairs)), 0, (DimentionsOfStairMesh.Z * dimensions.Z * i * VerticalSpacingInOpenStairs)));
				}
				else if (typeOfStairs == EtypeOfStairs::closeTypeStairs) {
					staircaseMeshComponent->SetRelativeScale3D(FVector((dimensions.X), dimensions.Y, dimensions.Z));
					staircaseMeshComponent->SetRelativeLocation(FVector((dimensions.X * DimentionsOfStairMesh.X * i), 0, (dimensions.Z * DimentionsOfStairMesh.Z * i)));
				}
				else {
					staircaseMeshComponent->SetRelativeScale3D(FVector(((dimensions.X) * sizeOfBoxStairs), dimensions.Y, dimensions.Z));
					staircaseMeshComponent->SetRelativeLocation(FVector(((dimensions.X * i * DimentionsOfStairMesh.X) / 2), 0, (dimensions.Z * DimentionsOfStairMesh.Z * i)));
				}

				Stair.Staircase = staircaseMeshComponent;
				staircaseMeshComponent->RegisterComponentWithWorld(GetWorld());
			}

			if (railing) {
				FString leftRailinigMeshName = "leftRailingComponent " + FString::FromInt(i);
				FString rightRailinigMeshName = "rightRailingComponent " + FString::FromInt(i);
				UStaticMeshComponent* leftRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *leftRailinigMeshName);
				UStaticMeshComponent* rightRailingStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *rightRailinigMeshName);

				if (leftRailingStaticMeshComponent) {
					leftRailingStaticMeshComponent->SetupAttachment(staircaseMeshComponent);
					leftRailingStaticMeshComponent->SetStaticMesh(railingMesh);
					if (typeOfStairs == EtypeOfStairs::boxTypeStairs) {
						leftRailingStaticMeshComponent->SetRelativeScale3D(FVector(leftRailingDimensions.X / sizeOfBoxStairs, leftRailingDimensions.Y, leftRailingDimensions.Z));
						leftRailingStaticMeshComponent->SetRelativeLocation(FVector((-(DimentionsOfStairMesh.X - DimentionsOfStairMesh.X / sizeOfBoxStairs)) / 2, -DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z));
					}
					else {
						leftRailingStaticMeshComponent->SetRelativeScale3D(leftRailingDimensions);
						leftRailingStaticMeshComponent->SetRelativeLocation(FVector(0, -DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z));
					}
					Stair.LeftRailing = leftRailingStaticMeshComponent;
					leftRailingStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
				}

				if (rightRailingStaticMeshComponent) {
					rightRailingStaticMeshComponent->SetupAttachment(staircaseMeshComponent);
					rightRailingStaticMeshComponent->SetStaticMesh(railingMesh);
					if (typeOfStairs == EtypeOfStairs::boxTypeStairs) {
						rightRailingStaticMeshComponent->SetRelativeScale3D(FVector(rightRailingDimensions.X / sizeOfBoxStairs, rightRailingDimensions.Y, rightRailingDimensions.Z));
						rightRailingStaticMeshComponent->SetRelativeLocation(FVector((-(DimentionsOfStairMesh.X - DimentionsOfStairMesh.X / sizeOfBoxStairs)) / 2, DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z));
					}
					else {
						rightRailingStaticMeshComponent->SetRelativeScale3D(rightRailingDimensions);
						rightRailingStaticMeshComponent->SetRelativeLocation(FVector(0, DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z));
					}
					Stair.RightRailing = rightRailingStaticMeshComponent;
					rightRailingStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
				}

				if (railingMesh) {
					DimentionsOfRailingMesh = railingMesh->GetBounds().GetBox().GetSize();
				}

				FString leftRailinigHandMeshName = "leftRailingHandComponent " + FString::FromInt(i);
				UStaticMeshComponent* leftRailingHandStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *leftRailinigHandMeshName);


				if (leftRailingHandStaticMeshComponent) {
					leftRailingHandStaticMeshComponent->SetupAttachment(staircaseMeshComponent);
					leftRailingHandStaticMeshComponent->SetStaticMesh(railingHandMesh);
					if (typeOfStairs == EtypeOfStairs::boxTypeStairs) {
						leftRailingHandStaticMeshComponent->SetRelativeScale3D(FVector(((dimensions.X / (cos(atan(dimensions.Z / dimensions.X)))) / dimensions.X) / sizeOfBoxStairs, leftRailingHandMeshDimensions.Y, leftRailingHandMeshDimensions.Z));
						leftRailingHandStaticMeshComponent->SetRelativeLocation(FVector((-(DimentionsOfStairMesh.X - DimentionsOfStairMesh.X / sizeOfBoxStairs)) / 2, -DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z + (DimentionsOfRailingMesh.Z * leftRailingDimensions.Z)));
						leftRailingHandStaticMeshComponent->SetRelativeRotation(FRotator(atan(dimensions.Z / dimensions.X) * 180 / 3.14, 0, 0));
					}
					else if (typeOfStairs == EtypeOfStairs::openTypeStairs) {
						double lenX = (dimensions.X * HorizontalSpacingInOpenStairs * DimentionsOfStairMesh.X) - (dimensions.X * DimentionsOfStairMesh.X);
						double lenZ = (dimensions.Z * VerticalSpacingInOpenStairs * DimentionsOfStairMesh.Z) - (dimensions.Z * DimentionsOfStairMesh.Z);
						double tanIn1 = atan(lenZ / lenX);
						double Cos2 = cos(tanIn1);
						double val2 = lenX / Cos2;
						double tanIn2 = atan((dimensions.Z * DimentionsOfStairMesh.Z) / (dimensions.X * DimentionsOfStairMesh.X));
						double Cos1 = cos(tanIn2);
						double val1 = (dimensions.X * DimentionsOfStairMesh.X) / Cos1;

						leftRailingHandStaticMeshComponent->SetRelativeScale3D(FVector(((val1 / (dimensions.X * DimentionsOfStairMesh.X)) + (val2 / (dimensions.X * DimentionsOfStairMesh.X))), leftRailingHandMeshDimensions.Y, leftRailingHandMeshDimensions.Z));
						leftRailingHandStaticMeshComponent->SetRelativeLocation(FVector(0, -DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z + (DimentionsOfRailingMesh.Z * leftRailingDimensions.Z)));
						leftRailingHandStaticMeshComponent->SetRelativeRotation(FRotator((atan(((dimensions.Z * DimentionsOfStairMesh.Z) * VerticalSpacingInOpenStairs) / ((dimensions.X * DimentionsOfStairMesh.X) * HorizontalSpacingInOpenStairs)) * 180 / 3.14), 0, 0));
					}
					else {
						leftRailingHandStaticMeshComponent->SetRelativeScale3D(FVector(((dimensions.X / (cos(atan(dimensions.Z / dimensions.X)))) / dimensions.X), leftRailingHandMeshDimensions.Y, leftRailingHandMeshDimensions.Z));
						leftRailingHandStaticMeshComponent->SetRelativeLocation(FVector(0, -DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z + (DimentionsOfRailingMesh.Z * leftRailingDimensions.Z)));
						leftRailingHandStaticMeshComponent->SetRelativeRotation(FRotator(atan(dimensions.Z / dimensions.X) * 180 / 3.14, 0, 0));
					}
				}
				Stair.LeftHandRailing = leftRailingHandStaticMeshComponent;
				leftRailingHandStaticMeshComponent->RegisterComponentWithWorld(GetWorld());

				FString rightRailinigHandMeshName = "rightRailingHandComponent " + FString::FromInt(i);
				UStaticMeshComponent* rightRailingHandStaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *rightRailinigHandMeshName);


				if (rightRailingHandStaticMeshComponent) {
					rightRailingHandStaticMeshComponent->SetupAttachment(staircaseMeshComponent);
					rightRailingHandStaticMeshComponent->SetStaticMesh(railingHandMesh);

					if (typeOfStairs == EtypeOfStairs::boxTypeStairs) {
						rightRailingHandStaticMeshComponent->SetRelativeScale3D(FVector(((dimensions.X / (cos(atan(dimensions.Z / dimensions.X)))) / dimensions.X) / sizeOfBoxStairs, rightRailingHandMeshDimensions.Y, rightRailingHandMeshDimensions.Z));
						rightRailingHandStaticMeshComponent->SetRelativeLocation(FVector((-(DimentionsOfStairMesh.X - DimentionsOfStairMesh.X / sizeOfBoxStairs)) / 2, DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z + (DimentionsOfRailingMesh.Z * leftRailingDimensions.Z)));
						rightRailingHandStaticMeshComponent->SetRelativeRotation(FRotator(atan(dimensions.Z / dimensions.X) * 180 / 3.14, 0, 0));
					}
					else if (typeOfStairs == EtypeOfStairs::openTypeStairs) {

						double lenX = (dimensions.X * HorizontalSpacingInOpenStairs * DimentionsOfStairMesh.X) - (dimensions.X * DimentionsOfStairMesh.X);
						double lenZ = (dimensions.Z * VerticalSpacingInOpenStairs * DimentionsOfStairMesh.Z) - (dimensions.Z * DimentionsOfStairMesh.Z);
						double tanIn1 = atan(lenZ / lenX);
						double Cos2 = cos(tanIn1);
						double val2 = lenX / Cos2;
						double tanIn2 = atan((dimensions.Z * DimentionsOfStairMesh.Z) / (dimensions.X * DimentionsOfStairMesh.X));
						double Cos1 = cos(tanIn2);
						double val1 = (dimensions.X * DimentionsOfStairMesh.X) / Cos1;

						rightRailingHandStaticMeshComponent->SetRelativeScale3D(FVector(((val1 / (dimensions.X * DimentionsOfStairMesh.X)) + (val2 / (dimensions.X * DimentionsOfStairMesh.X))), rightRailingHandMeshDimensions.Y, rightRailingHandMeshDimensions.Z));
					
						rightRailingHandStaticMeshComponent->SetRelativeLocation(FVector(0, DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z + (DimentionsOfRailingMesh.Z * leftRailingDimensions.Z)));
						rightRailingHandStaticMeshComponent->SetRelativeRotation(FRotator((atan(((dimensions.Z* DimentionsOfStairMesh.Z) * VerticalSpacingInOpenStairs) / ((dimensions.X * DimentionsOfStairMesh.X) * HorizontalSpacingInOpenStairs)) * 180 / 3.14), 0, 0));
					}
					else {
					rightRailingHandStaticMeshComponent->SetRelativeScale3D(FVector(((dimensions.X / (cos(atan(dimensions.Z / dimensions.X)))) / dimensions.X) * HorizontalSpacingInOpenStairs, rightRailingHandMeshDimensions.Y, rightRailingHandMeshDimensions.Z));
					rightRailingHandStaticMeshComponent->SetRelativeLocation(FVector(0, DimentionsOfStairMesh.Y / 2.3, DimentionsOfStairMesh.Z + (DimentionsOfRailingMesh.Z * leftRailingDimensions.Z)));
					rightRailingHandStaticMeshComponent->SetRelativeRotation(FRotator(atan(dimensions.Z / dimensions.X) * 180 / 3.14, 0, 0));
					}
				}
				Stair.RightHandRailing = rightRailingHandStaticMeshComponent;
				rightRailingHandStaticMeshComponent->RegisterComponentWithWorld(GetWorld());


			}
		}
		StaircaseComponents.Add(Stair);
		sizeOfBoxStairs--;
	}

}