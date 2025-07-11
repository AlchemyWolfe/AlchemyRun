// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipeComponent.h"
#include "Alchemist.h"
#include "Engine/World.h"

UAlchemyRecipeComponent::UAlchemyRecipeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAlchemyRecipeComponent::InitializeRecipe(int32 Seed, const AActor* Parent, UBoxComponent* NewFillBox)
{
	RandomStream.Initialize(Seed);
	AttachParent = GetOwner()->GetRootComponent();
	if (NewFillBox)
	{
		FillBox = NewFillBox;
	}
}

void UAlchemyRecipeComponent::GenerateStructureReagents()
{
	GenerateStructureReagentsImpl();
}

void UAlchemyRecipeComponent::GenerateRequiredReagents()
{
	GenerateRequiredReagentsImpl();
}

void UAlchemyRecipeComponent::GenerateDecorationReagents()
{
	GenerateDecorationReagentsImpl();
}

void UAlchemyRecipeComponent::BeginDestroy()
{
	ClearGeneratedActors();
	Super::BeginDestroy();
}

void UAlchemyRecipeComponent::GenerateStructureReagentsImpl()
{
}

void UAlchemyRecipeComponent::GenerateRequiredReagentsImpl()
{
	FVector Extent = FillBox->GetScaledBoxExtent();
	for (const FAlchemyReagentSpec& Spec : RequiredReagents)
	{
		FVector SpawnLocation;
		FRotator SpawnRotation;

	    UE_LOG(LogTemp, Warning, TEXT("UAlchemyRecipeComponent::GenerateRequiredReagentsImpl for %s : %s"), *Spec.NameTag, *Spec.ActorClass->GetName());
		if (AAlchemist::GetDesiredSpawnLocation(RandomStream, SpawnLocation, SpawnRotation,
				Extent, GenerationEdgeBuffer, Spec.PlacementLocation, Spec.PlacementFacing, -1))
		{
			AActor* Spawned = AAlchemist::SpawnReagent(
				Spec.ActorClass, Spec.NameTag, SpawnLocation, SpawnRotation, this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to place required reagent: %s"), *Spec.NameTag);
		}
	}
}

void UAlchemyRecipeComponent::GenerateDecorationReagentsImpl()
{
	FVector Extent = FillBox->GetScaledBoxExtent();
	for (const FAlchemyReagentSpec& Spec : DecorationReagents)
	{
		if (RandomStream.FRand() > 0.5f) // ~50% chance to place each decoration
		{
			FVector SpawnLocation;
			FRotator SpawnRotation;

			if (AAlchemist::GetDesiredSpawnLocation(RandomStream, SpawnLocation, SpawnRotation,
				Extent, 50.0f))
			{
				AActor* Spawned = AAlchemist::SpawnReagent(
					Spec.ActorClass, Spec.NameTag, SpawnLocation, SpawnRotation, this);
			}
		}
	}
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
}

AActor* UAlchemyRecipeComponent::FindReagent(const FString& SearchNameTag, const TSubclassOf<AActor>& SearchActorClass)
{
	for (int32 i = 0; i < GeneratedActors.Num(); ++i)
	{
		AActor* Actor = GeneratedActors[i];
		if (!Actor) continue;

		if (SearchActorClass && !Actor->IsA(SearchActorClass))
			continue;

		if (Actor->Tags.Contains(FName(*SearchNameTag)))
		{
			return Actor;
		}
	}
	return nullptr;
}

FVector UAlchemyRecipeComponent::GetActorClassBounds(const TSubclassOf<AActor>& ActorClass)
{
	if (!ActorClass) return FVector::ZeroVector;

	AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
	if (!DefaultActor) return FVector::ZeroVector;
	
	TInlineComponentArray<UPrimitiveComponent*> PrimitiveComponents;
	DefaultActor->GetComponents(PrimitiveComponents);

	bool bFoundBounds = false;
	FBox TotalBounds(ForceInit); // Initializes to invalid

	for (UPrimitiveComponent* Prim : PrimitiveComponents)
	{
		if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Prim))
		{
			if (UStaticMesh* Mesh = MeshComp->GetStaticMesh())
			{
				FBox MeshBounds = Mesh->GetBoundingBox();
				TotalBounds += MeshBounds;
				bFoundBounds = true;
			}
		}
		else
		{
			// Use default bounds if available
			FBox PrimBounds = Prim->CalcBounds(FTransform::Identity).GetBox();
			TotalBounds += PrimBounds;
			bFoundBounds = true;
		}
	}

	return bFoundBounds ? TotalBounds.GetSize() : FVector::ZeroVector;
}
