// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyReagent.h"
#include "AlchemyCauldron.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemyCauldron : public AAlchemyReagent
{
	GENERATED_BODY()

public:
	// Reagent entry data asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	UAlchemyReagentEntry* ReagentEntry;

};
