// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyBrickClass.h"
#include "AlchemyFillGridClass.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemyFillGridClass : public AAlchemyBrickClass
{
    GENERATED_BODY()

public:
    // Constructor
    AAlchemyFillGridClass();

protected:
    // Called when the game starts or the actor is spawned
    virtual void BeginPlay() override;

#if WITH_EDITOR
    // Called whenever a property changes in the editor
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
    // Class of the bricks to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSubclassOf<AAlchemyBrickClass> FillBrick;

    // Number of bricks in the grid
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FIntVector BrickCounts = FIntVector(1, 1, 1);

    // Record of all created bricks
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    TArray<AAlchemyBrickClass*> CreatedBricks;

    // Constructs the grid of bricks
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void ConstructGrid();

    // Destroys all created bricks
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void ClearGrid();

private:
    // Helper function to get the size of the brick
    FVector GetBrickSize() const;
};
