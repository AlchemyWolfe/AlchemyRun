// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyCatalog.h"
#include "AlchemyReagentEntry.h"
#include "Math/UnrealMathUtility.h"

TArray<UAlchemyReagentEntry*> UAlchemyCatalog::GetReagentsByAllTags(TArray<FName>& RequiredTags)
{
	TArray<UAlchemyReagentEntry*> MatchingReagents;
	for (UAlchemyReagentEntry* Reagent : Reagents)
	{
		bool bAllTagsMatch = true;
		for (const FName& Tag : RequiredTags)
		{
			if (!Reagent->Tags.Contains(Tag))
			{
				bAllTagsMatch = false;
				break;
			}
		}

		if (bAllTagsMatch)
		{
			MatchingReagents.Add(Reagent);
		}
	}
	return MatchingReagents;
}

UAlchemyReagentEntry* UAlchemyCatalog::GetRandomReagentFromList(TArray<UAlchemyReagentEntry*>& ReagentList)
{
	if (ReagentList.Num() == 0)
	{
		return nullptr;
	}
	int32 RandomIndex = FMath::RandRange(0, ReagentList.Num() - 1);
	return ReagentList[RandomIndex];
}
