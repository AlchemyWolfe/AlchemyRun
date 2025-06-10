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
    FVector FillOrigin = -FillExtent;
    FVector TileSize = FloorTile->GetBoundingBox().GetSize();
    USceneComponent* AttachParent = GetOwner()->GetRootComponent();

    // Derive grid size from tile if not already set
    GridSize = TileSize.X / FloorTileGridSpan;
    WallHeight = 2.0f * GridSize;

    int32 GridX = FMath::FloorToInt(FillExtent.X * 2 / TileSize.X) * FloorTileGridSpan;
    int32 GridY = FMath::FloorToInt(FillExtent.Y * 2 / TileSize.X) * FloorTileGridSpan;

    // Place floor tiles
    AActor* SpawnedActor = nullptr;
    FString MeshName;
    for (int32 X = 0; X < GridX; X += FloorTileGridSpan)
    {
        for (int32 Y = 0; Y < GridY; Y += FloorTileGridSpan)
        {
            MeshName = FString::Printf(TEXT("Floor_%d_%d"), X, Y);
            FVector Location = FillOrigin + FVector((X + 0.5f) * GridSize, (Y + 0.5f) * GridSize, -TileSize.Z);
            SpawnedActor = SpawnStaticMesh(FloorTile, AttachParent, MeshName, Location, FRotator::ZeroRotator);
        }
    }

    if (CornerTile != nullptr)
    {
        // Get corner rotations.  Done this way because complex use may ask for these more than once.
        TArray<FRotator> CornerRotations = {
            FRotator(0, 0, 0),
            FRotator(0, 90, 0),
            FRotator(0, 180, 0),
            FRotator(0, -90, 0)
        };
        // Place corners
        TArray<FVector> CornerLocations = {
            FillOrigin + FVector(CornerOffset.X, CornerOffset.Y, 0),
            FillOrigin + FVector(CornerOffset.X, CornerOffset.Y + 1/*GridY*/ * GridSize, 0),
            FillOrigin + FVector(CornerOffset.X + 1/*GridX*/ * GridSize, CornerOffset.Y, 0),
            FillOrigin + FVector(CornerOffset.X + 1/*GridX*/ * GridSize, CornerOffset.Y + 1/*GridY*/ * GridSize, 0)
        };

        for (int32 CornerIdx = 0; CornerIdx < CornerLocations.Num(); ++CornerIdx)
        {
            MeshName = FString::Printf(TEXT("Corner_%d"), CornerIdx);
            SpawnedActor = SpawnStaticMesh(CornerTile, AttachParent, MeshName, CornerLocations[0], CornerRotations[CornerIdx]);
        }
    }

    if (WallTile != nullptr)
    {
        /*
        // Place walls
        for (int32 X = 1; X < GridX; ++X)
        {
            FVector North = FillOrigin + FVector(X * GridSize, 0, 0);
            FVector South = FillOrigin + FVector(X * GridSize, GridY * GridSize, 0);
            MeshName = FString::Printf(TEXT("Wall_N_%d"), X);
            SpawnStaticMesh(WallTile, AttachParent, MeshName, North, FRotator(0, 90, 0));
            MeshName = FString::Printf(TEXT("Wall_S_%d"), X);
            SpawnStaticMesh(WallTile, AttachParent, MeshName, South, FRotator(0, -90, 0));
        }

        for (int32 Y = 1; Y < GridY; ++Y)
        {
            FVector West = FillOrigin + FVector(0, Y * GridSize, 0);
            FVector East = FillOrigin + FVector(GridX * GridSize, Y * GridSize, 0);
            MeshName = FString::Printf(TEXT("Wall_W_%d"), Y);
            SpawnStaticMesh(WallTile, AttachParent, MeshName, West, FRotator(0, 180, 0));
            MeshName = FString::Printf(TEXT("Wall_E_%d"), Y);
            SpawnStaticMesh(WallTile, AttachParent, MeshName, East, FRotator(0, 0, 0));
        }
        */
    }
}
