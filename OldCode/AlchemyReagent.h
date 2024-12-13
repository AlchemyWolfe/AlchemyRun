// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlchemyBaseDataAsset.h"
#include "AlchemyReagentEntry.h"
#include "AlchemyReagent.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemyReagent : public AActor
{
	GENERATED_BODY()

public:
	AAlchemyReagent();

	// Override BeginPlay to add custom generation logging
	virtual void BeginPlay() override;
};

