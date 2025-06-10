// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipe_TiledRoom.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

UAlchemyRecipe_TiledRoom::UAlchemyRecipe_TiledRoom()
{
    GridSize = 100.0f;
    FloorTileGridSpan = 1;
}

void UAlchemyRecipe_TiledRoom::GenerateReagentsImpl(AActor* Parent, UBoxComponent* FillBox)
{
    if (!FillBox)
    {
        return;
    }

}
