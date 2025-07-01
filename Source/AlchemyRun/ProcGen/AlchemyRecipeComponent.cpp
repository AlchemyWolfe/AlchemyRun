// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipeComponent.h"
#include "Alchemist.h"
#include "Engine/World.h"

UAlchemyRecipeComponent::UAlchemyRecipeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAlchemyRecipeComponent::InitializeRecipe(int32 Seed, const AActor* Parent, UBoxComponent* FillBox)
{
	RandomStream.Initialize(Seed);
	AttachParent = GetOwner()->GetRootComponent();
	FillExtent = FillBox->GetScaledBoxExtent();
	FillOrigin = -FillExtent;
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
}

void UAlchemyRecipeComponent::GenerateDecorationReagentsImpl()
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
}

AActor* UAlchemyRecipeComponent::SpawnReagent(UStaticMesh* Mesh, const FString& NameTag, const FVector& Location, const FRotator& Rotation, bool LocationIsLocal)
{
	if (!Mesh || !GetWorld()) return nullptr;

	FTransform ParentTransform = AttachParent->GetComponentTransform();
	FVector WorldLocation = LocationIsLocal ? ParentTransform.TransformPosition(Location) : Location;
	FRotator WorldRotation = LocationIsLocal ? ParentTransform.TransformRotation(Rotation.Quaternion()).Rotator() : Rotation;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Spawn empty actor
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), WorldLocation, WorldRotation, Params);
	if (!SpawnedActor) return nullptr;

	SpawnedActor->Tags.Add(FName(NameTag));

	// Create and attach mesh component
	UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(SpawnedActor);
	MeshComp->SetStaticMesh(Mesh);
	MeshComp->SetupAttachment(nullptr); // No parent yet
	MeshComp->SetWorldLocation(WorldLocation);
	MeshComp->SetWorldRotation(WorldRotation);
	MeshComp->RegisterComponent();

	// Set as root BEFORE attaching actor
	SpawnedActor->SetRootComponent(MeshComp);

	// Now attach to parent
	SpawnedActor->AttachToComponent(AttachParent, FAttachmentTransformRules::KeepWorldTransform);

	// Track the spawned actor
	GeneratedActors.Add(SpawnedActor);

	return SpawnedActor;
}

AActor* UAlchemyRecipeComponent::SpawnReagent(const TSubclassOf<AActor>& ActorClass,
	const FString& NameTag, const FVector& Location, const FRotator& Rotation, bool LocationIsLocal)
{
	if (!GetWorld() || !ActorClass || !AttachParent) return nullptr;

	FTransform ParentTransform = AttachParent->GetComponentTransform();
	FVector WorldLocation = LocationIsLocal ? ParentTransform.TransformPosition(Location) : Location;
	FRotator WorldRotation = LocationIsLocal ? ParentTransform.TransformRotation(Rotation.Quaternion()).Rotator() : Rotation;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, WorldLocation, WorldRotation, Params);
	if (!SpawnedActor) return nullptr;
	SpawnedActor->Tags.Add(FName(NameTag));

	SpawnedActor->AttachToComponent(AttachParent, FAttachmentTransformRules::KeepWorldTransform);
	GeneratedActors.Add(SpawnedActor);

	// Call Blueprint event: OnReagentSpawned(int32)
	UFunction* InitFunc = SpawnedActor->FindFunction(FName("OnReagentSpawned"));
	if (InitFunc)
	{
		struct
		{
			int32 Seed;
		} SpawnedParams;
		SpawnedParams.Seed = static_cast<int32>(RandomStream.GetUnsignedInt());

		SpawnedActor->ProcessEvent(InitFunc, &SpawnedParams);
	}
	
	return SpawnedActor;
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

AActor* UAlchemyRecipeComponent::SpawnReagentInPlaceOf(const FString& NewNameTag,
                                                       const TSubclassOf<AActor>& NewActorClass, AActor* OldReagent)
{
	if (!NewActorClass || !OldReagent) return nullptr;

	FVector Location = OldReagent->GetActorLocation();
	FRotator Rotation = OldReagent->GetActorRotation();

	// Destroy the old actor and remove it from the list
	auto i = GeneratedActors.Find(OldReagent);
	GeneratedActors.RemoveAt(i);
	OldReagent->Destroy();

	// Spawn and return the replacement
	return SpawnReagent(NewActorClass, NewNameTag, Location, Rotation, false);
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
