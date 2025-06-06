// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipeComponent.h"
#include "Engine/World.h"

UAlchemyRecipeComponent::UAlchemyRecipeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bGenerated = false;
}

void UAlchemyRecipeComponent::GenerateReagents(int32 Seed, AActor* Parent, UBoxComponent* FillBox)
{
	RandomStream.Initialize(Seed);
	ClearGeneratedActors();
	GenerateReagentsImpl(Parent, FillBox);
	bGenerated = true;
}

void UAlchemyRecipeComponent::BeginDestroy()
{
	ClearGeneratedActors();
	Super::BeginDestroy();
}

void UAlchemyRecipeComponent::GenerateReagentsImpl(AActor* Parent, UBoxComponent* FillBox)
{
}

void UAlchemyRecipeComponent::ClearGeneratedActors()
{
	for (AActor* Actor : GeneratedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	GeneratedActors.Empty();
	bGenerated = false;
}

AActor* UAlchemyRecipeComponent::SpawnStaticMesh(UStaticMesh* Mesh, FVector Location, FRotator Rotation)
{
	if (!Mesh || !GetWorld()) return nullptr;

	FActorSpawnParameters Params;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), Location, Rotation, Params);
	if (!SpawnedActor) return nullptr;

	UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(SpawnedActor);
	MeshComp->SetStaticMesh(Mesh);
	MeshComp->RegisterComponent();
	MeshComp->AttachToComponent(SpawnedActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	SpawnedActor->SetRootComponent(MeshComp);
	GeneratedActors.Add(SpawnedActor);

	return SpawnedActor;
}
