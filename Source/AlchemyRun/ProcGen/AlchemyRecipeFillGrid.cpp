// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyRecipeFillGrid.h"
#include "AlchemyReagentEntry.h"
#include "AlchemyCauldron.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

void UAlchemyRecipeFillGrid::GenerateReagents(AAlchemyReagent* Parent/*, const FCollisionShape& Footprint*/)
{
    /*
    if (!FillBrick)
    {
        UE_LOG(LogTemp, Error, TEXT("FillBrick is not set for UAlchemyRecipeFillGrid."));
        return;
    }

    if (!Parent)
    {
        UE_LOG(LogTemp, Error, TEXT("Parent is not set for UAlchemyRecipeFillGrid."));
        return;
    }

    if (!Parent->IsA<AAlchemyCauldron>())
    {
        UE_LOG(LogTemp, Log, TEXT("Creating a new AlchemyCauldron as parent."));
        Parent = Parent->GetWorld()->SpawnActor<AAlchemyCauldron>();
        Parent->SetActorLocation(Footprint.GetExtent());
    }

    FVector Origin, Extent;
    Footprint.GetBoxExtent(Extent);
    Origin = Parent->GetActorLocation() - Extent;

    FIntVector BlockCount = FIntVector(
        FMath::CeilToInt(Extent.X * 2 / FillBrick->Footprint.GetBox().GetExtent().X),
        FMath::CeilToInt(Extent.Y * 2 / FillBrick->Footprint.GetBox().GetExtent().Y),
        FMath::CeilToInt(Extent.Z * 2 / FillBrick->Footprint.GetBox().GetExtent().Z)
    );

    for (int32 X = 0; X < BlockCount.X; X++)
    {
        for (int32 Y = 0; Y < BlockCount.Y; Y++)
        {
            for (int32 Z = 0; Z < BlockCount.Z; Z++)
            {
                FVector Location = Origin + FVector(
                    X * FillBrick->Footprint.GetBox().GetExtent().X,
                    Y * FillBrick->Footprint.GetBox().GetExtent().Y,
                    Z * FillBrick->Footprint.GetBox().GetExtent().Z
                );

                AAlchemyReagent* NewReagent = FillBrick->SpawnActor(Parent, Location, FRotator::ZeroRotator);
                if (NewReagent)
                {
                    CreatedReagents.Add(NewReagent);
                    UE_LOG(LogTemp, Log, TEXT("Generated AAlchemyReagent at location: %s"), *Location.ToString());
                }
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Finished generating reagents. Total generated: %d"), CreatedReagents.Num());
    */
}
