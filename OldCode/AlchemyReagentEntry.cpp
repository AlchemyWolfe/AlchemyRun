// Copyright Alchemy Wolfe.  All Rights Reserved.


#include "AlchemyReagentEntry.h"
#include "AlchemyReagent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AAlchemyReagent* UAlchemyReagentEntry::SpawnActor(AActor* Parent, float RotationIncrement, FVector RelativeLocation, FRotator RelativeRotation)
{
	if (!ActorReference.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorReference is not valid!"));
		return nullptr;
	}

	UWorld* World = Parent->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is not valid!"));
		return nullptr;
	}

	// Get the class from the soft reference
	UClass* ActorClass = ActorReference.Get();
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorReference did not resolve to a valid class!"));
		return nullptr;
	}

	// If RotationIncrement is not zero, calculate a random rotation around Y
	if (RotationIncrement != 0.0f)
	{
		int32 NumIncrements = FMath::FloorToInt(360.0f / RotationIncrement);
		int32 RandomIncrement = FMath::RandRange(0, NumIncrements - 1);
		float YawRotation = RandomIncrement * RotationIncrement;

		RelativeRotation.Yaw += YawRotation; // Apply the random rotation to the yaw
	}
	
	// Spawn the actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Parent;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector WorldLocation = Parent->GetActorLocation() + RelativeLocation;

	AAlchemyReagent* NewActor = World->SpawnActor<AAlchemyReagent>(ActorClass, WorldLocation, RelativeRotation, SpawnParams);

	return NewActor;
}
