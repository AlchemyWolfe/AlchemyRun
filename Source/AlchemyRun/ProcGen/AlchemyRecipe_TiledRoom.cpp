// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyRecipe_TiledRoom.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UnrealTypePrivate.h"
#include "UObject/UnrealType.h"
#include "UObject/Class.h"

UAlchemyRecipe_TiledRoom::UAlchemyRecipe_TiledRoom()
{
    GridSize = 100.0f;
}

void UAlchemyRecipe_TiledRoom::InitializeRecipe(int32 Seed, const AActor* Parent, UBoxComponent* FillBox)
{
    Super::InitializeRecipe(Seed, Parent, FillBox);

    FloorTileSize = FillExtent;

    if (FloorActor)
    {
        // Try to get the default object for bounds info (not spawned yet)
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* TempActor = GetWorld()->SpawnActor<AActor>(FloorActor, FVector(0, 0, -100000), FRotator::ZeroRotator, Params);
        if (TempActor)
        {
            FloorTileSize = TempActor->GetComponentsBoundingBox().GetSize();
            TempActor->Destroy();
        }
    }
    
    GridSize = FloorTileSize.X;
    GridMaxX = static_cast<int32>(FillExtent.X * 2.f / GridSize);
    GridMaxY = static_cast<int32>(FillExtent.Y * 2.f / GridSize);

    GridInitialized = true;
}

void UAlchemyRecipe_TiledRoom::GenerateStructureReagentsImpl()
{
    FillGrid(FloorActor, 0, 0, GridMaxX, GridMaxY, 0);
    FillRing(WallActor, CornerActor, 0, 0, GridMaxX, GridMaxY, 0);
}

void UAlchemyRecipe_TiledRoom::GenerateRequiredReagentsImpl()
{
}

void UAlchemyRecipe_TiledRoom::GenerateDecorationReagentsImpl()
{
}

void UAlchemyRecipe_TiledRoom::FillGrid(const TSubclassOf<AActor>& ActorClass, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }
    
    FString NameTag;
    for (int32 X = MinX; X <= MaxX; ++X)
    {
        for (int32 Y = MinY; Y <= MaxY; ++Y)
        {
            FVector Location = GetGridLocation(X, Y, Z);
            NameTag = FString::Printf(TEXT("Floor_%d_%d"), X, Y);
            SpawnReagent(ActorClass, NameTag, Location, FRotator::ZeroRotator);
        }
    }
}

void UAlchemyRecipe_TiledRoom::FillRing(const TSubclassOf<AActor>& WallActorClass,
    const TSubclassOf<AActor>& CornerActorClass, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z)
{
    int cornerBuffer = 0;
    if (CornerActorClass != nullptr)
    {
        SpawnSWCorner(CornerActorClass, MinX, MinY, Z);
        SpawnNWCorner(CornerActorClass, MaxX, MinY, Z);
        SpawnNECorner(CornerActorClass, MaxX, MaxY, Z);
        SpawnSECorner(CornerActorClass, MinX, MaxY, Z);
        cornerBuffer = 1;
    }
    FillNorthWall(WallActorClass, MaxX, MinY+cornerBuffer, MaxY-cornerBuffer, Z);
    FillSouthWall(WallActorClass, MinX, MinY+cornerBuffer, MaxY-cornerBuffer, Z);
    FillEastWall(WallActorClass, MaxY, MinX+cornerBuffer, MaxX-cornerBuffer, Z);
    FillWestWall(WallActorClass, MinY, MinX+cornerBuffer, MaxX-cornerBuffer, Z);
}

AActor* UAlchemyRecipe_TiledRoom::AddDoorwayInWall(int X, int Y, bool DoorExists)
{
    FString NameTag = FString::Printf(TEXT("WallActor_%d_%d"), X, Y);
    AActor* OldWallActor = FindReagent(NameTag, WallActor);
    if (OldWallActor != nullptr)
    {
        NameTag = FString::Printf(TEXT("DoorwayActor_%d_%d"), X, Y);
        AActor* NewDoorwayActor = SpawnReagentInPlaceOf(NameTag, DoorwayActor, OldWallActor);
        if (NewDoorwayActor != nullptr && DoorExists)
        {
            FName VarName("DoorExists");
            FProperty* Property = NewDoorwayActor->GetClass()->FindPropertyByName("DoorExists");
            FBoolProperty* BoolProp = CastField<FBoolProperty>(Property);
            if (BoolProp)
            {
                BoolProp->SetPropertyValue_InContainer(NewDoorwayActor, true);
            }
        }
        return NewDoorwayActor;
    }
    return nullptr;
}

AActor* UAlchemyRecipe_TiledRoom::RemoveDoorwayInWall(int X, int Y)
{
    FString NameTag = FString::Printf(TEXT("DoorwayActor_%d_%d"), X, Y);
    AActor* OldDoorwayActor = FindReagent(NameTag, DoorwayActor);
    if (OldDoorwayActor != nullptr)
    {
        NameTag = FString::Printf(TEXT("WallActor_%d_%d"), X, Y);
        AActor* NewWallActor = SpawnReagentInPlaceOf(NameTag, WallActor, OldDoorwayActor);
        return NewWallActor;
    }
    return nullptr;
}

void UAlchemyRecipe_TiledRoom::FillNorthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString NameTag;
    FRotator LocalRotation = FRotator(0, 180, 0);
    for (int32 Y = MinY; Y <= MaxY; Y++)
    {
        FVector Location = GetGridLocation(X+1, Y+1, Z);
        NameTag = FString::Printf(TEXT("WallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, NameTag, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::FillSouthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString NameTag;
    FRotator LocalRotation = FRotator(0, 0, 0);
    for (int32 Y = MinY; Y <= MaxY; Y++)
    {
        FVector Location = GetGridLocation(X, Y, Z);
        NameTag = FString::Printf(TEXT("WallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, NameTag, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::FillEastWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString SpawnName;
    FRotator LocalRotation = FRotator(0, -90, 0);
    for (int32 X = MinX; X <= MaxX; X++)
    {
        FVector Location = GetGridLocation(X, Y+1, Z);
        SpawnName = FString::Printf(TEXT("WallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, SpawnName, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::FillWestWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FString SpawnName;
    FRotator LocalRotation = FRotator(0, 90, 0);
    for (int32 X = MinX; X <= MaxX; X++)
    {
        FVector Location = GetGridLocation(X+1, Y, Z);
        SpawnName = FString::Printf(TEXT("WallActor_%d_%d"), X, Y);
        SpawnReagent(ActorClass, SpawnName, Location, LocalRotation);
    }
}

void UAlchemyRecipe_TiledRoom::SpawnSWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X, Y, Z);;
    FString SpawnName = FString::Printf(TEXT("SWCornerActor"));
    SpawnReagent(ActorClass, SpawnName, Location, FRotator(0, 0, 0));
}

void UAlchemyRecipe_TiledRoom::SpawnNWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X+1, Y, Z);
    FString SpawnName = FString::Printf(TEXT("NWCornerActor"));
    SpawnReagent(ActorClass, SpawnName, Location, FRotator(0, 90, 0));
}

void UAlchemyRecipe_TiledRoom::SpawnNECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X+1, Y+1, Z);
    FString SpawnName = FString::Printf(TEXT("NECornerActor"));
    SpawnReagent(ActorClass, SpawnName, Location, FRotator(0, 180, 0));
}

void UAlchemyRecipe_TiledRoom::SpawnSECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z)
{
    if (ActorClass == nullptr)
    {
        return;
    }

    FVector Location = GetGridLocation(X, Y+1, Z);
    FString SpawnName = FString::Printf(TEXT("SECornerActor"));
    SpawnReagent(ActorClass, SpawnName, Location, FRotator(0, -90, 0));
}
