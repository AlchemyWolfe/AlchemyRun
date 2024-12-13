// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyCatalog.h"

UAlchemyCatalog::UAlchemyCatalog()
{
}

void UAlchemyCatalog::AddReagentToTagList(TSoftObjectPtr<AActor> Reagent, const FName& Tag)
{
	if (Reagent.IsValid())  // Ensure the reagent is valid
	{
		if (!TagIndex.Contains(Tag))
		{
			TagIndex.Add(Tag, TSet<TSoftObjectPtr<AActor>>());
		}
		TagIndex[Tag].Add(Reagent);
	}
}

TArray<TSoftObjectPtr<AActor>> UAlchemyCatalog::GetReagentsByTags(const TArray<FName>& Tags) const
{
	TArray<TSoftObjectPtr<AActor>> Result;

	// If there are no tags, return an empty result
	if (Tags.Num() == 0)
	{
		return Result;
	}

	// Start with the set of reagents for the first tag
	TSet<TSoftObjectPtr<AActor>> ReagentsSet = TagIndex.Contains(Tags[0]) ? TagIndex[Tags[0]] : TSet<TSoftObjectPtr<AActor>>();

	// Intersect with reagents for each subsequent tag
	for (int32 i = 1; i < Tags.Num(); i++)
	{
		if (TagIndex.Contains(Tags[i]))
		{
			ReagentsSet = ReagentsSet.Intersect(TagIndex[Tags[i]]);
		}
	}

	// Convert the TSet to a TArray for the result
	Result.Append(ReagentsSet.Array());

	return Result;
}
