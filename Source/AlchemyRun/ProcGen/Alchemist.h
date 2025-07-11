// Copyright Alchemy Wolfe. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "AlchemyEnums.h"
#include "AlchemyRecipeComponent.h"
#include "Alchemist.generated.h"

// Why isn't this a match constant?
#define SQRT_2_INV 0.70710678118

UCLASS()
class ALCHEMYRUN_API AAlchemist : public AActor
{
    GENERATED_BODY()

public:
    AAlchemist();
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ContentAlchemy", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* FillBox;

    UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
    int32 GenerationSeed;

    // Random stream for deterministic generation
    static FRandomStream RandomStream;

    // Creates a Reagent Actor from a StaticMesh
    static AActor* SpawnReagent(UStaticMesh* Mesh, const FString& NameTag, const FVector& Location, const FRotator& Rotation, UAlchemyRecipeComponent* Recipe);

    // Creates a Reagent Actor from an ActorClass
    static AActor* SpawnReagent(const TSubclassOf<AActor>& ActorClass, const FString& NameTag, const FVector& Location, const FRotator& Rotation, UAlchemyRecipeComponent* Recipe, bool LocationIsLocal = true);

    // Finds an actor with a specific NameTag, deletes it, and spawns a new actor in its place.  Returns null if no actor is found.
    static AActor* FindReagent(UAlchemyRecipeComponent* Recipe, const FString& SearchNameTag, const TSubclassOf<AActor>& SearchActorClass = nullptr);

    // Finds an actor with a specific NameTag, deletes it, and spawns a new actor in its place.  Returns null if no actor is found.
    static AActor* SpawnReagentInPlaceOf(UAlchemyRecipeComponent* Recipe, AActor* OldReagent, const FString& NewNameTag, const TSubclassOf<AActor>& NewActorClass);

    // Gets a desired location ignoring collision.
    static bool GetDesiredSpawnLocation(
        FRandomStream& Random,
        FVector& OutLocalLocation,
        FRotator& OutLocalRotation,
        const FVector& Extent,
        const float Buffer,
        EReagentPlacementLocation Location = EReagentPlacementLocation::Anywhere,
        EReagentPlacementFacing Facing = EReagentPlacementFacing::Any,
        float Position = 0.0f
    );
    
protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

private:
    UFUNCTION(CallInEditor, Category = "ContentAlchemy")
    void RegenerateLevel();
    UFUNCTION(CallInEditor, Category = "ContentAlchemy")
    void ClearLevel();

    void EnqueueTopLevelRecipies();
    void GenerateLevel();
    void ClearGeneratedActors();
    void GenerateBreadthFirst();
    void ProcessRecipe(UAlchemyRecipeComponent* Recipe);
    void PlaySpawnAnimation(AActor* Actor);

    UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
    float AnimationDuration;

    UPROPERTY(EditAnywhere, Category = "ContentAlchemy")
    float SpawnDelay;

protected:
    TQueue<UAlchemyRecipeComponent*> RecipeQueue;
};

