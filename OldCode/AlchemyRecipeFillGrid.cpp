// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyRecipeFillGrid.h"
#include "AlchemyReagentEntry.h"
#include "AlchemyCauldron.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AAlchemyReagent* UAlchemyRecipeFillGrid::GenerateReagents(int32 Seed, AAlchemyReagent* Parent, UBoxComponent* FillBox, const FString& CauldronName)
{
    // Return without generating if FillBrick or FillBox is null.
    if (!FillBrick || !FillBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("FillBrick or FillBox is null!"));
        return Parent;
    }

    // Call the base class GenerateReagents.
    Parent = Super::GenerateReagents(Seed, Parent, FillBox, CauldronName);

    // Get the dimensions of the FillBox.
    FVector FillBoxExtent = FillBox->GetScaledBoxExtent() * 2.0f; // Total size of the box.
    FVector FillBoxCenter = FillBox->GetComponentLocation();

    // Assuming FillBrick has a Footprint size, calculate how many can fit.
    FVector BrickSize = FVector::ZeroVector;
    if (FillBrick->Footprint)
    {
        BrickSize = FillBrick->Footprint->GetScaledBoxExtent() * 2.0f;
    }

    if (BrickSize.X <= 0 || BrickSize.Y <= 0 || BrickSize.Z <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("FillBrick's Footprint size is invalid! Using defaults."));
        BrickSize = FVector(100.0f, 100.0f, 100.0f); // Default size if none is specified.
    }

    // Calculate the number of bricks in each dimension (minimum 1).
    int32 NumX = FMath::Max(1, FMath::FloorToInt(FillBoxExtent.X / BrickSize.X));
    int32 NumY = FMath::Max(1, FMath::FloorToInt(FillBoxExtent.Y / BrickSize.Y));
    int32 NumZ = FMath::Max(1, FMath::FloorToInt(FillBoxExtent.Z / BrickSize.Z));

    FVector StartLocation = FillBoxCenter;
    StartLocation.X -= (BrickSize.X * NumX) * 0.5f;
    StartLocation.Y -= (BrickSize.Y * NumY) * 0.5f;
    StartLocation.Z -= (BrickSize.Z * NumZ) * 0.5f;
    FVector BrickLocation = StartLocation;
    
    // Loop through x, y, z, and spawn FillBrick at each grid point.
    for (int32 x = 0; x < NumX; ++x)
    {
        BrickLocation.Y = StartLocation.Y;
        for (int32 y = 0; y < NumY; ++y)
        {
            BrickLocation.Z = StartLocation.Z;
            for (int32 z = 0; z < NumZ; ++z)
            {
                // Spawn a FillBrick.
                AAlchemyReagent* Brick = FillBrick->SpawnActor(Parent, FillBrickRotationIncrement, BrickLocation);

                // Optionally spawn a decoration as a child of the FillBrick.
                if (Brick && Decorations.Num() > 0 && RandomStream.FRand() < DecorationChance)
                {
                    int32 RandomIndex = RandomStream.RandRange(0, Decorations.Num() - 1);
                    UAlchemyReagentEntry* Decoration = Decorations[RandomIndex];

                    if (Decoration)
                    {
                        Decoration->SpawnActor(Brick, DecorationRotationIncrement);
                    }
                }
                BrickLocation.Z += BrickSize.Z;
            }
            BrickLocation.Y += BrickSize.Y;
        }
        BrickLocation.X += BrickSize.X;
    }

    return Parent;
}
