// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipeComponent.h"
#include "Alchemist.h"
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

AActor* UAlchemyRecipeComponent::SpawnStaticMesh(UStaticMesh* Mesh, USceneComponent* AttachParent, const FString& Name, FVector LocalLocation, FRotator LocalRotation)
{
	if (!Mesh || !GetWorld()) return nullptr;

	FTransform ParentTransform = AttachParent->GetComponentTransform();
	FVector WorldLocation = ParentTransform.TransformPosition(LocalLocation);
	FRotator WorldRotation = ParentTransform.TransformRotation(LocalRotation.Quaternion()).Rotator();

	FActorSpawnParameters Params;
	Params.Name = FName(*FString::Printf(TEXT("%s_%d"), *Name, AAlchemist::GetNextUniqueId()));
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Spawn empty actor
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), WorldLocation, WorldRotation, Params);
	if (!SpawnedActor) return nullptr;

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

AActor* UAlchemyRecipeComponent::SpawnActor(const TSubclassOf<AActor>& ActorClass, USceneComponent* AttachParent,
	const FString& Name, const FVector& LocalLocation, const FRotator& LocalRotation)
{
	if (!GetWorld() || !ActorClass || !AttachParent) return nullptr;

	FTransform ParentTransform = AttachParent->GetComponentTransform();
	FVector WorldLocation = ParentTransform.TransformPosition(LocalLocation);
	FRotator WorldRotation = ParentTransform.TransformRotation(LocalRotation.Quaternion()).Rotator();

	FActorSpawnParameters Params;
	Params.Name = FName(*FString::Printf(TEXT("%s_%d"), *Name, AAlchemist::GetNextUniqueId()));
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, WorldLocation, WorldRotation, Params);
	if (!SpawnedActor) return nullptr;

	SpawnedActor->AttachToComponent(AttachParent, FAttachmentTransformRules::KeepWorldTransform);
	GeneratedActors.Add(SpawnedActor);

	return SpawnedActor;
}
