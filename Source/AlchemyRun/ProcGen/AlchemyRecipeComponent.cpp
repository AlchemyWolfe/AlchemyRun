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

AActor* UAlchemyRecipeComponent::SpawnReagent(float RotationIncrement, FVector RelativeLocation,
	FRotator RelativeRotation)
{
	FActorSpawnParameters SpawnParams;
	return nullptr;//GetWorld()->SpawnActor<AActor>(SomeActorClass, Location, Rotation, SpawnParams);
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
