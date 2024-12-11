// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyRecipe.h"
#include "AlchemyReagentEntry.h"
#include "CollisionShape.h"
#include "AlchemyRecipeFillGrid.generated.h"

/**
 * A recipe that generates a specific FillBrick AlchemyReagent and tiles it within the given space.
 */
UCLASS()
class ALCHEMYRUN_API UAlchemyRecipeFillGrid : public UAlchemyRecipe
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
	UAlchemyReagentEntry* FillBrick; // Called brick because it implies rectangular tiling.

	/**
	 * Generates children reagents in a grid pattern within the given space.
	 * @param Parent The parent reagent for the generated children.
	 * @param Footprint The area within which the reagents should be generated.
	 */
	virtual void GenerateReagents(AAlchemyReagent* Parent/*, const FCollisionShape& Footprint*/) override;
};
