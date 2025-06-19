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

    InitializeGridValues(Parent, FillBox);

    // Place tiles
    FVector FloorTileSize = FloorSM->GetBoundingBox().GetSize();
    FillGrid(FloorSM, 0, 0, GridMaxX, GridMaxY, -FloorTileSize.Z, FloorTileGridSpan);
    SpawnSWCorner(CornerActor, 0, 0, 0);
    SpawnNWCorner(CornerActor, GridMaxX, 0, 0);
    SpawnNECorner(CornerActor, GridMaxX, GridMaxY, 0);
    SpawnSECorner(CornerActor, 0, GridMaxY, 0);
    FillNorthWall(WallActor, GridMaxX, 1, GridMaxY-1, 0);
    FillSouthWall(WallActor, 0, 1, GridMaxY-1, 0);
    FillEastWall(WallActor, GridMaxY, 1, GridMaxX-1, 0);
    FillWestWall(WallActor, 0, 1, GridMaxX-1, 0);
}

void UAlchemyRecipe_TiledRoom::InitializeGridValues(const AActor* Parent, const UBoxComponent* FillBox)
{
    AttachParent = GetOwner()->GetRootComponent();
    FillExtent = FillBox->GetScaledBoxExtent();
    FillOrigin = -FillExtent;
    //FillOrigin.Z += FillExtent.Z * 0.5f;
    TileSize = FillExtent;

    if (FloorSM != nullptr)
    {
        TileSize = FloorSM->GetBoundingBox().GetSize();
    }
    else if (FloorActor)
    {
        // Try to get the default object for bounds info (not spawned yet)
        AActor* DefaultActor = FloorActor->GetDefaultObject<AActor>();
        if (DefaultActor)
        {
            UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(DefaultActor->GetRootComponent());
            if (PrimComp)
            {
                TileSize = PrimComp->Bounds.GetBox().GetSize();
            }
        }
    }
    
    GridSize = TileSize.X / FloorTileGridSpan;
    GridMaxX = static_cast<int32>(FillExtent.X * 2.f / TileSize.X) * FloorTileGridSpan - 1;
    GridMaxY = static_cast<int32>(FillExtent.Y * 2.f / TileSize.X) * FloorTileGridSpan - 1;

    GridInitialized = true;
}

void UAlchemyRecipe_TiledRoom::FillGrid(UStaticMesh* Mesh, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z, int32 Step)
{
    if (Mesh == nullptr)
    {
        return;
    }
    
    FString SpawnName;
    for (int32 X = MinX; X <= MaxX; X += Step)
    {
        for (int32 Y = MinY; Y <= MaxY; Y += Step)
        {
            FVector Location = GetGridLocation(X, Y, Z);
            SpawnName = FString::Printf(TEXT("FloorSM_%d_%d"), X, Y);
            SpawnReagent(Mesh, AttachParent, SpawnName, Location, FRotator::ZeroRotator);
        }
    }
}

void UAlchemyRecipe_TiledRoom::FillGrid(const TSubclassOf<AActor>& ActorClass, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z, int32 Step)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::FillNorthWall(UStaticMesh* Mesh, int32 X, int32 MinY, int32 MaxY, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::FillNorthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString SpawnName;
    FRotator LocalRotation = FRotator(0, 180, 0);
    for (int32 Y = MinY; Y <= MaxY; Y++)
    {
        FVector Location = GetGridLocation(X+1, Y, Z);
        SpawnName = FString::Printf(TEXT("NWallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, AttachParent, SpawnName, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::FillSouthWall(UStaticMesh* Mesh, int32 X, int32 MinY, int32 MaxY, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::FillSouthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString SpawnName;
    FRotator LocalRotation = FRotator(0, 0, 0);
    for (int32 Y = MinY; Y <= MaxY; Y++)
    {
        FVector Location = GetGridLocation(X, Y+1, Z);
        SpawnName = FString::Printf(TEXT("NWallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, AttachParent, SpawnName, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::FillEastWall(UStaticMesh* Mesh, int32 Y, int32 MinX, int32 MaxX, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::FillEastWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString SpawnName;
    FRotator LocalRotation = FRotator(0, 90, 0);
    for (int32 X = MinX; X <= MaxX; X++)
    {
        FVector Location = GetGridLocation(X, Y+1, Z);
        SpawnName = FString::Printf(TEXT("EWallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, AttachParent, SpawnName, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::FillWestWall(UStaticMesh* Mesh, int32 Y, int32 MinX, int32 MaxX, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::FillWestWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString SpawnName;
    FRotator LocalRotation = FRotator(0, -90, 0);
    for (int32 X = MinX; X <= MaxX; X++)
    {
        FVector Location = GetGridLocation(X+1, Y, Z);
        SpawnName = FString::Printf(TEXT("EWallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, AttachParent, SpawnName, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::SpawnSWCorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::SpawnSWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X, Y, Z);;
    FString SpawnName = FString::Printf(TEXT("SWCornerActor"));
    SpawnReagent(ActorClass, AttachParent, SpawnName, Location, FRotator(0, 0, 0));
}

void UAlchemyRecipe_TiledRoom::SpawnNWCorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::SpawnNWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X+1, Y, Z);
    FString SpawnName = FString::Printf(TEXT("NWCornerActor"));
    SpawnReagent(ActorClass, AttachParent, SpawnName, Location, FRotator(0, 90, 0));
}

void UAlchemyRecipe_TiledRoom::SpawnNECorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::SpawnNECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X+1, Y+1, Z);
    FString SpawnName = FString::Printf(TEXT("NECornerActor"));
    SpawnReagent(ActorClass, AttachParent, SpawnName, Location, FRotator(0, 180, 0));
}

void UAlchemyRecipe_TiledRoom::SpawnSECorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z)
{
    // Create is I ever use it.
}

void UAlchemyRecipe_TiledRoom::SpawnSECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X, Y+1, Z);
    FString SpawnName = FString::Printf(TEXT("SECornerActor"));
    SpawnReagent(ActorClass, AttachParent, SpawnName, Location, FRotator(0, -90, 0));
}

