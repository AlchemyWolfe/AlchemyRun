// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CollisionShape.h"
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
	TArray<AAlchemyReagent*> CreatedReagents;

	// Clear created reagents
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	virtual void ClearReagents();

	// Generate reagents within a given footprint
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	virtual void GenerateReagents(AAlchemyReagent* Parent/*, FCollisionShape Footprint*/);
};
