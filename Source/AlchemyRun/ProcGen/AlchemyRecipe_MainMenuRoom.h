// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyRecipe_TiledRoom.h"
#include "AlchemyRecipe_MainMenuRoom.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ALCHEMYRUN_API UAlchemyRecipe_MainMenuRoom : public UAlchemyRecipe_TiledRoom
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> StairsActor;
	
protected:
	virtual void GenerateStructureReagentsImpl() override;
	virtual void GenerateRequiredReagentsImpl() override;
	virtual void GenerateDecorationReagentsImpl() override;
};
