// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyFillGridClass.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Constructor
AAlchemyFillGridClass::AAlchemyFillGridClass()
{
    PrimaryActorTick.bCanEverTick = false; // No need for ticking
}

// Called when the game starts
void AAlchemyFillGridClass::BeginPlay()
{
    Super::BeginPlay();
    ConstructGrid();
}

// Handles changes to properties in the editor
#if WITH_EDITOR
void AAlchemyFillGridClass::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Automatically construct the grid when relevant properties change
    ConstructGrid();
}
#endif

// Constructs the grid of bricks
void AAlchemyFillGridClass::ConstructGrid()
{
    ClearGrid(); // Clear any existing bricks

    if (!FillBrick) return; // Ensure the FillBrick class is set

    FVector BrickSize = GetBrickSize(); // Get the size of the brick
    FVector StartOffset(0.f, 0.f, 0.f);

    // Ensure minimum values for BrickCounts
    BrickCounts.X = FMath::Max(BrickCounts.X, 1);
    BrickCounts.Y = FMath::Max(BrickCounts.Y, 1);
    BrickCounts.Z = FMath::Max(BrickCounts.Z, 1);

    // Calculate the offset for centering
    StartOffset = -0.5f * FVector(
        (BrickCounts.X - 1) * BrickSize.X,
        (BrickCounts.Y - 1) * BrickSize.Y,
        (BrickCounts.Z - 1)* BrickSize.Z
    );

    UWorld* World = GetWorld();
    if (!World) return;

    // Spawn the bricks
    for (int32 X = 0; X < BrickCounts.X; ++X)
    {
        for (int32 Y = 0; Y < BrickCounts.Y; ++Y)
        {
            for (int32 Z = 0; Z < BrickCounts.Z; ++Z)
            {
                // Calculate the spawn location
                FVector LocalPosition = StartOffset + FVector(X * BrickSize.X, Y * BrickSize.Y, Z * BrickSize.Z);

                // Spawn parameters
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;

                // Spawn the brick
                AAlchemyBrickClass* NewBrick = World->SpawnActor<AAlchemyBrickClass>(FillBrick, LocalPosition, FRotator::ZeroRotator, SpawnParams);
                if (NewBrick)
                {
                    // Attach the brick to this actor
                    NewBrick->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

                    // Set its relative position and rotation
                    NewBrick->SetActorRelativeLocation(LocalPosition);
                    NewBrick->SetActorRelativeRotation(FRotator::ZeroRotator);
                    NewBrick->SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

                    // Add to the array of created bricks
                    CreatedBricks.Add(NewBrick);
                }
            }
        }
    }
}

// Destroys all created bricks
void AAlchemyFillGridClass::ClearGrid()
{
    for (AAlchemyBrickClass* Brick : CreatedBricks)
    {
        if (Brick)
        {
            Brick->Destroy();
        }
    }
    CreatedBricks.Empty();
}

// Helper function to get the size of the brick
FVector AAlchemyFillGridClass::GetBrickSize() const
{
    if (FillBrick)
    {
        AAlchemyBrickClass* DefaultBrick = Cast<AAlchemyBrickClass>(FillBrick->GetDefaultObject());
        if (DefaultBrick)
        {
            return DefaultBrick->Size;
        }
    }
    return FVector(100.f, 100.f, 100.f); // Default size
}

