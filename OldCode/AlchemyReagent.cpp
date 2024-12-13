// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyReagent.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogAlchemy, Log, All);

AAlchemyReagent::AAlchemyReagent()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAlchemyReagent::BeginPlay()
{
	Super::BeginPlay();

	// Log the generation of the reagent
	UE_LOG(LogAlchemy, Log, TEXT("AAlchemyReagent '%s' generated at location: %s"), *GetName(), *GetActorLocation().ToString());
}
