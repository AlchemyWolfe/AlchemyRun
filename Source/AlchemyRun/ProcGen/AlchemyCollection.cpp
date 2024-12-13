// Copyright Alchemy Wolfe. All Rights Reserved.

// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyCollection.h"
#include "Engine/World.h"
#include "Math/RandomStream.h"

// Constructor
UAlchemyCollection::UAlchemyCollection()
{
    // Initialize the Reagents array with the number of rarities (this will work even if rarities are added in the future)
    int32 NumRarities = static_cast<int32>(EAlchemyCollectionRarity::MaxRarities);
    Reagents.SetNum(NumRarities);

    // Set the weight for each rarity (0.01 * previous weight)
    // For Common (index 0), set the weight to 1.0f
    float PreviousChance = 1.0f;

    // Loop through all rarities and assign the weight accordingly
    for (int32 RarityIndex = 0; RarityIndex < NumRarities; ++RarityIndex)
    {
        Reagents[RarityIndex].Chance = PreviousChance;
        PreviousChance *= 0.01f;  // Reduce weight for the next rarity
    }
}

EAlchemyCollectionRarity UAlchemyCollection::GetRandomRarity(FRandomStream& RandomStream) const
{
    // Generate a random number between 0 and the 1
    float RandomChance = RandomStream.FRand();

    // Find which rarity that has entries corresponds to the generated random number
    for (int32 RarityIndex = Reagents.Num() - 1; RarityIndex >= 0; --RarityIndex)
    {
        if (Reagents[RarityIndex].Reagents.Num() > 0 && RandomChance <= Reagents[RarityIndex].Chance)
        {
            return static_cast<EAlchemyCollectionRarity>(RarityIndex);
        }
    }

    return EAlchemyCollectionRarity::Common;    // We should never get here, but this is a safe return value.
}

// Get a random reagent of any rarity based on the weights of each rarity
TSoftObjectPtr<AActor> UAlchemyCollection::GetRandomReagent(FRandomStream& RandomStream) const
{
    EAlchemyCollectionRarity Rarity = GetRandomRarity(RandomStream);
    return GetRandomReagentOfRarity(RandomStream, Rarity);
}

// Get a random reagent of a specific rarity
TSoftObjectPtr<AActor> UAlchemyCollection::GetRandomReagentOfRarity(FRandomStream& RandomStream, EAlchemyCollectionRarity Rarity) const
{
    // Get the appropriate FAlchemyCollectionRarityData for the given rarity
    const FAlchemyCollectionRarityData& RarityData = Reagents[static_cast<int32>(Rarity)];

    // If the array of reagents for this rarity is not empty, pick a random one
    if (RarityData.Reagents.Num() > 0)
    {
        int32 RandomIndex = RandomStream.RandRange(0, RarityData.Reagents.Num() - 1);
        return RarityData.Reagents[RandomIndex];
    }

    // If no reagents, return a null pointer
    return TSoftObjectPtr<AActor>();
}

// Add a reagent to a specific rarity
void UAlchemyCollection::AddReagent(TSoftObjectPtr<AActor> Reagent, EAlchemyCollectionRarity Rarity)
{
    // Add the reagent to the corresponding rarity array in the Reagents list
    Reagents[static_cast<int32>(Rarity)].Reagents.Add(Reagent);
}

void UAlchemyCollection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Calculate the number of rarities based on the enum
    int32 NumRarities = static_cast<int32>(EAlchemyCollectionRarity::MaxRarities);

    // If the number of rarities has increased, extend the Reagents array
    if (Reagents.Num() < NumRarities)
    {
        int32 MissingRarities = NumRarities - Reagents.Num();
        for (int32 i = 0; i < MissingRarities; ++i)
        {
            // Add a new entry with default values for the new rarity
            FAlchemyCollectionRarityData NewRarityData;
            NewRarityData.Chance = 0.01f * Reagents.Last().Chance;  // Use a small weight value for new rarity
            Reagents.Add(NewRarityData);
        }

        // Notify the designer that a new rarity has been added
        UE_LOG(LogTemp, Warning, TEXT("New rarity added to AlchemyCollection: %s"), *GetName());
    }
}
