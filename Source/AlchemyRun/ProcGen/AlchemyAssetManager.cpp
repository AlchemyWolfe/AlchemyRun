// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyAssetManager.h"
#include "AlchemyReagentEntry.h"
#include "AlchemyCatalog.h"
#include "Engine/Engine.h"

UAlchemyAssetManager& UAlchemyAssetManager::Get()
{
	UAlchemyAssetManager* This = Cast<UAlchemyAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
        UE_LOG(LogTemp, Fatal, TEXT("Invalid AlchemyReagentManager in DefaultEngine.ini, Must set this to AlchemyReagentManager"));
		return *NewObject<UAlchemyAssetManager>();
	}
}

TArray<UAlchemyReagentEntry*> UAlchemyAssetManager::GetReagentsByAllTags(const TArray<FName>& RequiredTags)
{
    TArray<UAlchemyReagentEntry*> MatchingReagents;
    // Logic to find matching reagents
    return MatchingReagents;
}

TArray<UAlchemyReagentEntry*> UAlchemyAssetManager::GetReagentsByAnyTag(const TArray<FName>& Tags)
{
    TArray<UAlchemyReagentEntry*> MatchingReagents;
    // Logic to find matching reagents
    return MatchingReagents;
}

UAlchemyReagentEntry* UAlchemyAssetManager::GetRandomReagentByAllTags(const TArray<FName>& RequiredTags)
{
    TArray<UAlchemyReagentEntry*> Reagents = GetReagentsByAllTags(RequiredTags);
    if (Reagents.Num() == 0)
    {
        return nullptr;
    }
    int32 RandomIndex = FMath::RandRange(0, Reagents.Num() - 1);
    return Reagents[RandomIndex];
}

UAlchemyReagentEntry* UAlchemyAssetManager::GetRandomReagentByAnyTag(const TArray<FName>& Tags)
{
    TArray<UAlchemyReagentEntry*> Reagents = GetReagentsByAnyTag(Tags);
    if (Reagents.Num() == 0)
    {
        return nullptr;
    }
    int32 RandomIndex = FMath::RandRange(0, Reagents.Num() - 1);
    return Reagents[RandomIndex];
}

TArray<UAlchemyCatalog*> UAlchemyAssetManager::GetCatalogsByAllTags(const TArray<FName>& RequiredTags)
{
    TArray<UAlchemyCatalog*> MatchingCatalogs;
    // Logic to find matching catalogs
    return MatchingCatalogs;
}

TArray<UAlchemyCatalog*> UAlchemyAssetManager::GetCatalogsByAnyTag(const TArray<FName>& Tags)
{
    TArray<UAlchemyCatalog*> MatchingCatalogs;
    // Logic to find matching catalogs
    return MatchingCatalogs;
}

UAlchemyCatalog* UAlchemyAssetManager::GetRandomCatalogByAllTags(const TArray<FName>& RequiredTags)
{
    TArray<UAlchemyCatalog*> Catalogs = GetCatalogsByAllTags(RequiredTags);
    if (Catalogs.Num() == 0)
    {
        return nullptr;
    }
    int32 RandomIndex = FMath::RandRange(0, Catalogs.Num() - 1);
    return Catalogs[RandomIndex];
}

UAlchemyCatalog* UAlchemyAssetManager::GetRandomCatalogByAnyTag(const TArray<FName>& Tags)
{
    TArray<UAlchemyCatalog*> Catalogs = GetCatalogsByAnyTag(Tags);
    if (Catalogs.Num() == 0)
    {
        return nullptr;
    }
    int32 RandomIndex = FMath::RandRange(0, Catalogs.Num() - 1);
    return Catalogs[RandomIndex];
}
