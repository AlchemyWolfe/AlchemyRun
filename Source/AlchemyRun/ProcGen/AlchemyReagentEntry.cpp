// Copyright Alchemy Wolfe.  All Rights Reserved.


#include "AlchemyReagentEntry.h"
#include "AlchemyReagent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AAlchemyReagent* UAlchemyReagentEntry::SpawnActor(AActor* Parent, FVector RelativeLocation, FRotator RelativeRotation)
{
	if (!ActorReference.IsValid())
	{
		return nullptr;
	}

	UWorld* World = Parent->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Parent;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAlchemyReagent* NewActor = nullptr;//World->SpawnActor<AAlchemyReagent>(ActorReference.Get(), Parent->GetActorLocation() + RelativeLocation, RelativeRotation, SpawnParams);

	return NewActor;
}
