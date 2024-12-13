// Copyright Alchemy Wolfe. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AlchemyCatalog.generated.h"

// The point of an Alchemy Catalog is to have a place to group Reagents by tag, for retrieval in a recipe that uses
// the reagents as a group.  An example of this would be a group of architecture pieces meant to be used together.
// I expect two uses for this.
// One would be creating aesthetically matching pieces.
// Another would be for creating a short list of Reagents from AlchemyCatalogs, before starting a large generation.  An
// example of this would be choosing the contents of a tavern, and then tagging one asset "chair" so that all chairs in
// the tavern matched.
// Currently, all Reagents are AActors.  If we want the flexibility of using Collections as Reagents, then all we need
// to do to this class is change the type of TSoftObjectPtr we use.

UCLASS(Blueprintable)
class ALCHEMYRUN_API UAlchemyCatalog : public UObject
{
	GENERATED_BODY()

public:
	UAlchemyCatalog();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AlchemyCatalog")
	TArray<FName> AvailableTags;  // Tags available in this catalog

	UFUNCTION(BlueprintCallable, Category = "AlchemyCatalog")
	void AddReagentToTagList(TSoftObjectPtr<AActor> Reagent, const FName& Tag);

	UFUNCTION(BlueprintCallable, Category = "AlchemyCatalog")
	TArray<TSoftObjectPtr<AActor>> GetReagentsByTags(const TArray<FName>& Tags) const;

protected:
	TMap<FName, TSet<TSoftObjectPtr<AActor>>> TagIndex; // Tag -> Reagents
};
