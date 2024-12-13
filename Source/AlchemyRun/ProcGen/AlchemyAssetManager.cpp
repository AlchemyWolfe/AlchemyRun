// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyAssetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UAlchemyAssetManager& UAlchemyAssetManager::Get()
{
    UAlchemyAssetManager* Instance = Cast<UAlchemyAssetManager>(GEngine->AssetManager);
    if (!Instance)
    {
        UE_LOG(LogTemp, Fatal, TEXT("AlchemyAssetManager is not initialized properly. Make sure it is set in the project settings."));
        return *NewObject<UAlchemyAssetManager>();
    }
    return *Instance;
}

void UAlchemyAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    // Placeholder for preloading catalogs or collections
    UE_LOG(LogTemp, Log, TEXT("AlchemyAssetManager: Initial loading started."));
}

UObject* UAlchemyAssetManager::LoadAssetSynchronously(const FString& AssetPath)
{
    FSoftObjectPath SoftObjectPath(AssetPath);
    UObject* LoadedAsset = SoftObjectPath.TryLoad();
    if (!LoadedAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to synchronously load asset: %s"), *AssetPath);
    }
    return LoadedAsset;
}

void UAlchemyAssetManager::LoadAssetAsynchronously(const FString& AssetPath, TFunction<void(UObject*)> OnLoadComplete)
{
    FSoftObjectPath SoftObjectPath(AssetPath);

    //FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    StreamableManager.RequestAsyncLoad(SoftObjectPath, [OnLoadComplete, SoftObjectPath]()
    {
        UObject* LoadedAsset = SoftObjectPath.ResolveObject();
        if (LoadedAsset)
        {
            OnLoadComplete(LoadedAsset);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to asynchronously load asset: %s"), *SoftObjectPath.ToString());
            OnLoadComplete(nullptr);
        }
    });
}
