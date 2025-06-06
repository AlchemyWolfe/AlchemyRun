// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipe_TiledRoomAutosized.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

UAlchemyRecipe_TiledRoomAutosized::UAlchemyRecipe_TiledRoomAutosized()
{
    GridSize = 256.0f;
    WallHeight = 512.0f;
    FloorTileGridSpan = 3;
}

void UAlchemyRecipe_TiledRoomAutosized::GenerateReagentsImpl(AActor* Parent, UBoxComponent* FillBox)
{
    if (!FillBox || !FloorTile)
    {
        return;
    }

    FVector FillExtent = FillBox->GetScaledBoxExtent();
    FVector FillOrigin = FillBox->GetComponentLocation() - FillExtent;
    FVector TileSize = FloorTile->GetBoundingBox().GetSize();

    // Derive grid size from tile if not already set
    GridSize = TileSize.X;
    WallHeight = 2.0f * GridSize;

    int32 GridX = FMath::FloorToInt(FillExtent.X * 2 / GridSize);
    int32 GridY = FMath::FloorToInt(FillExtent.Y * 2 / GridSize);

    // Place floor tiles
    for (int32 X = 0; X < GridX; ++X)
    {
        for (int32 Y = 0; Y < GridY; ++Y)
        {
            FVector Location = FillOrigin + FVector((X + 0.5f) * GridSize, (Y + 0.5f) * GridSize, 0);
            SpawnStaticMesh(FloorTile, Location, FRotator::ZeroRotator);
        }
    }

    // Place corners
    TArray<FVector2D> Corners = {
        FVector2D(0, 0), FVector2D(0, GridY),
        FVector2D(GridX, 0), FVector2D(GridX, GridY)
    };

    for (FVector2D Corner : Corners)
    {
        FVector Location = FillOrigin + FVector(Corner.X * GridSize, Corner.Y * GridSize, 0);
        float Rotation = (Corner.X == 0 ? 180.f : 0.f) + (Corner.Y == 0 ? 90.f : -90.f);
        SpawnStaticMesh(CornerTile, Location, FRotator(0, Rotation, 0));
    }

    // Place walls
    for (int32 X = 1; X < GridX; ++X)
    {
        FVector North = FillOrigin + FVector(X * GridSize, 0, 0);
        FVector South = FillOrigin + FVector(X * GridSize, GridY * GridSize, 0);
        SpawnStaticMesh(WallTile, North, FRotator(0, 90, 0));
        SpawnStaticMesh(WallTile, South, FRotator(0, -90, 0));
    }

    for (int32 Y = 1; Y < GridY; ++Y)
    {
        FVector West = FillOrigin + FVector(0, Y * GridSize, 0);
        FVector East = FillOrigin + FVector(GridX * GridSize, Y * GridSize, 0);
        SpawnStaticMesh(WallTile, West, FRotator(0, 180, 0));
        SpawnStaticMesh(WallTile, East, FRotator(0, 0, 0));
    }
}
