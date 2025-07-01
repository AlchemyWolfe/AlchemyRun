// Copyright Alchemy Wolfe. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "AlchemyRecipeComponent.h"
#include "Alchemist.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemist : public AActor
{
    GENERATED_BODY()

public:
    AAlchemist();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* FillBox;

    UPROPERTY(EditAnywhere, Category = "Generation")
    int32 GenerationSeed;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

private:
    UFUNCTION(CallInEditor, Category = "Generation")
    void RegenerateLevel();
    UFUNCTION(CallInEditor, Category = "Generation")
    void ClearLevel();

    void EnqueueTopLevelRecipies();
    void GenerateLevel();
    void ClearGeneratedActors();
    void GenerateBreadthFirst();
    void ProcessRecipe(UAlchemyRecipeComponent* Recipe);
    void PlaySpawnAnimation(AActor* Actor);

    // Random stream for deterministic generation
    FRandomStream RandomStream;

    UPROPERTY(EditAnywhere, Category = "Generation")
    float AnimationDuration;

    UPROPERTY(EditAnywhere, Category = "Generation")
    float SpawnDelay;

protected:
    TQueue<UAlchemyRecipeComponent*> RecipeQueue;
};

