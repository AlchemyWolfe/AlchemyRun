// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyBrickClass.h"
#include "Math/UnrealMathUtility.h"

// Constructor
AAlchemyBrickClass::AAlchemyBrickClass()
{
    PrimaryActorTick.bCanEverTick = false; // No need for ticking unless required
}

// Called when the game starts
void AAlchemyBrickClass::BeginPlay()
{
    Super::BeginPlay();

    // Call the initial spawn logic
    InitialSpawn();
}

// Default implementation of InitialSpawn
void AAlchemyBrickClass::InitialSpawn_Implementation()
{
    // By default, call RotateRandomly
    RotateRandomly();
}

// Rotates the brick randomly around the Z-axis
void AAlchemyBrickClass::RotateRandomly()
{
    if (RandomRotationIncrement <= 0.5f) return;

    // Generate a random increment count
    RotationIncrementCount = CalculateRandomIncrementCount();

    // Apply the rotation
    float RotationAngle = RotationIncrementCount * RandomRotationIncrement;
    FRotator NewRotation(0.f, 0.f, RotationAngle);
    SetActorRotation(NewRotation);
}

// Helper function to calculate random rotation increment count
int32 AAlchemyBrickClass::CalculateRandomIncrementCount() const
{
    int32 MaxIncrements = FMath::FloorToInt(360.f / RandomRotationIncrement);
    return FMath::RandRange(0, MaxIncrements-1);
}
