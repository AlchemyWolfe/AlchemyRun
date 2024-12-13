// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyBaseDataAsset.h"
#include "CollisionShape.h"
#include "Components/BoxComponent.h"
#include "AlchemyReagentEntry.generated.h"

class UAlchemyRecipe;
class AAlchemyReagent;

UCLASS(Blueprintable)
class ALCHEMYRUN_API UAlchemyReagentEntry : public UAlchemyBaseDataAsset
{
	GENERATED_BODY()

public:
	// Name for organization/logging purposes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	FName Name;

	// Tags defining this reagent for lookup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TArray<FName> Tags;

	// Reference to the Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TSoftClassPtr<AAlchemyReagent> ActorReference;
	
	// Footprint for collision shape (limits placement area)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	UBoxComponent* Footprint;

	// The recipe used for generating additional content
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	UAlchemyRecipe* Recipe;

	// Method to spawn actor based on reagent entry
	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	AAlchemyReagent* SpawnActor(AActor* Parent, float RotationIncrement = 0.0f, FVector RelativeLocation = FVector::ZeroVector, FRotator RelativeRotation = FRotator::ZeroRotator);
};
