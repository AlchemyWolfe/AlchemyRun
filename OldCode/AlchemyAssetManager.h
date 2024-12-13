// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AlchemyReagentEntry.h"
#include "AlchemyCatalog.h"
#include "AlchemyAssetManager.generated.h"

/**
 * Asset manager for handling Alchemy assets.
 */
UCLASS()
class ALCHEMYRUN_API UAlchemyAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    /**
     * Get the singleton instance of the Alchemy Asset Manager.
     */
    static UAlchemyAssetManager& Get();

    /**
     * Get reagents by matching all required tags.
     * @param RequiredTags The tags that all reagents must have.
     * @return An array of reagents that match all the required tags.
     */
    TArray<UAlchemyReagentEntry*> GetReagentsByAllTags(const TArray<FName>& RequiredTags);

    /**
     * Get reagents by matching any of the tags.
     * @param Tags The tags that reagents may have.
     * @return An array of reagents that match any of the tags.
     */
    TArray<UAlchemyReagentEntry*> GetReagentsByAnyTag(const TArray<FName>& Tags);

    /**
     * Get a random reagent from those matching all required tags.
     * @param RequiredTags The tags that all reagents must have.
     * @return A random reagent that matches all the required tags.
     */
    UAlchemyReagentEntry* GetRandomReagentByAllTags(const TArray<FName>& RequiredTags);

    /**
     * Get a random reagent from those matching any of the tags.
     * @param Tags The tags that reagents may have.
     * @return A random reagent that matches any of the tags.
     */
    UAlchemyReagentEntry* GetRandomReagentByAnyTag(const TArray<FName>& Tags);

    /**
     * Get catalogs by matching all required tags.
     * @param RequiredTags The tags that all catalogs must have.
     * @return An array of catalogs that match all the required tags.
     */
    TArray<UAlchemyCatalog*> GetCatalogsByAllTags(const TArray<FName>& RequiredTags);

    /**
     * Get catalogs by matching any of the tags.
     * @param Tags The tags that catalogs may have.
     * @return An array of catalogs that match any of the tags.
     */
    TArray<UAlchemyCatalog*> GetCatalogsByAnyTag(const TArray<FName>& Tags);

    /**
     * Get a random catalog from those matching all required tags.
     * @param RequiredTags The tags that all catalogs must have.
     * @return A random catalog that matches all the required tags.
     */
    UAlchemyCatalog* GetRandomCatalogByAllTags(const TArray<FName>& RequiredTags);

    /**
     * Get a random catalog from those matching any of the tags.
     * @param Tags The tags that catalogs may have.
     * @return A random catalog that matches any of the tags.
     */
    UAlchemyCatalog* GetRandomCatalogByAnyTag(const TArray<FName>& Tags);

protected:
};
