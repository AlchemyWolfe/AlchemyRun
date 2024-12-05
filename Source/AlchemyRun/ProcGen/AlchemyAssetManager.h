// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AlchemyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYRUN_API UAlchemyAssetManager : public UAssetManager
{
	GENERATED_BODY()

	UAlchemyAssetManager() {};

	virtual void StartInitialLoading() override;

	static const FPrimaryAssetType AlchemyReagentType;
	static const FPrimaryAssetType AlchemyCatalogType;

	// return the asset manager singleton class
	static UAlchemyAssetManager& Get();
};
