// Copyright Alchemy Wolfe.  All Rights Reserved.
// Copyright Alchemy Wolfe. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "AlchemyCollection.generated.h"

// Enum to define the rarity of the assets, only used inside UAlchemyCollection
UENUM(BlueprintType)
enum class EAlchemyCollectionRarity : uint8
{
    Common UMETA(DisplayName = "Common"),
    Rare UMETA(DisplayName = "Rare"),
    SuperRare UMETA(DisplayName = "Super Rare"),
    MaxRarities UMETA(DisplayName = "Max Rarities")
};

// Only used inside UAlchemyCollection, this is just meant to sort Reagents into rarities, so random lookup
// doesn't have to walk the entire list counting weights.
USTRUCT(BlueprintType)
struct FAlchemyCollectionRarityData
{
    GENERATED_BODY()

public:
    // List of Reagents for this rarity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AlchemyCollection")
    TArray<TSoftObjectPtr<AActor>> Reagents;

    // Not exactly the weight.  We get a random from 0.0 to 0.1, and check the smallest chances first, by enum order.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AlchemyCollection")
    float Chance = 0.1f;
};

// The point of an Alchemy Collection is to have a place to group Reagents of the same type based on a kind of rarity.
// I expect this to be used to make a short list of Reagents to create a temporary AlchemyCatalog out of.
// This temporary AlchemyCatalog will then be passed to child Recipes, so there is some consistency in generation.
// It is possible also to create a short list of Reagents as a Collection to be passed to child Recipes.
// Currently, all Reagents are AActors.  If we want the flexibility of using Collections as Reagents, then all we need
// to do to this class is change the type of TSoftObjectPtr we use.
UCLASS(Blueprintable)
class ALCHEMYRUN_API UAlchemyCollection : public UObject
{
    GENERATED_BODY()
    
    UAlchemyCollection();

public:
    // Each rarity will have one list of Reagents for each rarity.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AlchemyCollection")
    TArray<FAlchemyCollectionRarityData> Reagents;

    // Returns a random rarity based on rarity chance
    UFUNCTION(BlueprintCallable, Category = "AlchemyCollection")
    EAlchemyCollectionRarity GetRandomRarity(FRandomStream& RandomStream) const;

    // Returns a Reagent of any rarity.
    UFUNCTION(BlueprintCallable, Category = "AlchemyCollection")
    TSoftObjectPtr<AActor> GetRandomReagent(FRandomStream& RandomStream) const;
    
    // Returns a Reagent of a specific rarity.
    UFUNCTION(BlueprintCallable, Category = "AlchemyCollection")
    TSoftObjectPtr<AActor> GetRandomReagentOfRarity(FRandomStream& RandomStream, EAlchemyCollectionRarity Rarity) const;

    // Adds an actor to the given rarity collection.  I only see doing this to make a smaller curated list.
    UFUNCTION(BlueprintCallable, Category = "AlchemyCollection")
    void AddReagent(TSoftObjectPtr<AActor> Reagent, EAlchemyCollectionRarity Rarity);
    
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
