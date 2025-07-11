// Copyright Alchemy Wolfe.  All Rights Reserved.
// Copyright Alchemy Wolfe. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlchemyEnums.h"
#include "AlchemyCollection.generated.h"

USTRUCT(BlueprintType)
struct FAlchemyActorEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Weight = 1.0f;
};

UCLASS()
class ALCHEMYRUN_API AAlchemyCollection : public AActor
{
    GENERATED_BODY()

public:
    AAlchemyCollection();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
    TArray<FAlchemyActorEntry> ActorEntries;
    
    // Resolves nested AAlchemyReagent selections until a final AActor class is chosen
    static TSubclassOf<AActor> GetChosenActorClass(TSubclassOf<AActor> ReagentClass, FRandomStream& RandomStream);
    
    TSubclassOf<AActor> GetChosenActorClass(FRandomStream& RandomStream);
    
protected:
    // Returns a random actor class from the collection, filtered by optional MaxRarity
    UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
    TSubclassOf<AActor> GetRandomActorClass(FRandomStream& RandomStream);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    // A collection, dropped into the world, will replace itself with one of the available actors, or destroy itself.
    bool SuccessfullyReplaced;
    int ReplaceAttempts;
    
    UFUNCTION(CallInEditor, Category = "ContentAlchemy")
    void ReplaceThisActorWithOneFromActorEntries();
};
