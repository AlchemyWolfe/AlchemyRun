// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyAssetManager.h"

const FPrimaryAssetType UAlchemyAssetManager::AlchemyReagentType = TEXT("AlchemyReagent");
const FPrimaryAssetType UAlchemyAssetManager::AlchemyCatalogType = TEXT("AlchemyCatalog");

void UAlchemyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

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
