// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AlchemyRecipe.generated.h"

class UAlchemyCatalog;
class AAlchemyReagent;

UCLASS(Blueprintable)
class ALCHEMYRUN_API UAlchemyRecipe : public UObject
{
	GENERATED_BODY()

public:
	// Recipe name for logging purposes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	FName Name;

	// Tags used by the recipe to get reagents and catalogs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TArray<FName> RecipeTags;

	// Shared catalogs available for this recipe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TArray<UAlchemyCatalog*> SharedCatalogs;

	// List of reagents created by this recipe
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ContentAlchemy")
	TArray<AActor*> CreatedReagents;

	// Clear created reagents
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	void ClearReagents();

	// Creates an Actor from an AlchemyReagentEntry
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	AAlchemyReagent* SpawnReagent(UAlchemyReagentEntry* ReagentEntry, float RotationIncrement = 0.0f, FVector RelativeLocation = FVector::ZeroVector, FRotator RelativeRotation = FRotator::ZeroRotator);

	// Generate reagents within a given footprint
	// Returns the Parent it used.  Some recipes may create a Cauldron for grouping.
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	virtual AAlchemyReagent* GenerateReagents(int32 Seed, AAlchemyReagent* Parent, UBoxComponent* FillBox, const FString& CauldronName);

protected:
	AAlchemyReagent* ParentReagent;
	FRandomStream RandomStream;
};
