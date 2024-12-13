// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyBaseDataAsset.h"
#include "AlchemyCatalog.generated.h"

class UAlchemyReagentEntry;

UCLASS(Blueprintable)
class ALCHEMYRUN_API UAlchemyCatalog : public UAlchemyBaseDataAsset
{
	GENERATED_BODY()

public:
	// Tags to identify this catalog
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TArray<FName> Tags;

	// List of reagents in this catalog
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TArray<UAlchemyReagentEntry*> Reagents;

	// Get reagents by all required tags
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	TArray<UAlchemyReagentEntry*> GetReagentsByAllTags(TArray<FName>& RequiredTags);

	// Get random reagent based on tags
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	static UAlchemyReagentEntry* GetRandomReagentFromList(TArray<UAlchemyReagentEntry*>& ReagentList);
};
