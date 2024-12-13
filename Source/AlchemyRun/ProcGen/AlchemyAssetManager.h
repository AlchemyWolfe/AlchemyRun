// Copyright Alchemy Wolfe. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AlchemyAssetManager.generated.h"

/**
 * The AlchemyAssetManager is responsible for managing and streaming assets used in procedural generation.
 * This includes loading necessary actors, catalogs, and collections dynamically.
 */
UCLASS()
class ALCHEMYRUN_API UAlchemyAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    /** Singleton access to the AlchemyAssetManager */
    static UAlchemyAssetManager& Get();

    /**
     * Load an asset synchronously by its path.
     * @param AssetPath The path to the asset to load.
     * @return A pointer to the loaded UObject, or nullptr if loading failed.
     */
    UObject* LoadAssetSynchronously(const FString& AssetPath);

    /**
     * Load an asset asynchronously by its path.
     * @param AssetPath The path to the asset to load.
     * @param OnLoadComplete Callback for when the asset is loaded.
     */
    void LoadAssetAsynchronously(const FString& AssetPath, TFunction<void(UObject*)> OnLoadComplete);

protected:
    virtual void StartInitialLoading() override;
};
