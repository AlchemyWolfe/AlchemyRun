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

	UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
	float FillBrickRotationIncrement = 90.0f;

	UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
	TArray<UAlchemyReagentEntry*> Decorations;

	UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
	float DecorationChance = 0.2f;

	UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
	float DecorationRotationIncrement = 90.0f;

	virtual AAlchemyReagent* GenerateReagents(int32 Seed, AAlchemyReagent* Parent, UBoxComponent* FillBox, const FString& CauldronName) override;
};
