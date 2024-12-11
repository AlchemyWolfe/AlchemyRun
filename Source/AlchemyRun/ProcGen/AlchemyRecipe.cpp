// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyRecipe.h"
#include "AlchemyReagent.h"
#include "AlchemyReagentEntry.h"

void UAlchemyRecipe::ClearReagents()
{
	for (AAlchemyReagent* Reagent : CreatedReagents)
	{
		if (Reagent)
		{
			Reagent->Destroy();
		}
	}
	CreatedReagents.Empty();
}

void UAlchemyRecipe::GenerateReagents(AAlchemyReagent* Parent/*, const FCollisionShape& Footprint*/)
{
	// Placeholder for the actual generation logic
}
