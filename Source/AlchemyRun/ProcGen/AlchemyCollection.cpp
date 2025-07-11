// Copyright Alchemy Wolfe. All Rights Reserved.

#include "AlchemyCollection.h"

#include "Alchemist.h"
#include "AlchemyReagent.h"

AAlchemyCollection::AAlchemyCollection()
{
    PrimaryActorTick.bCanEverTick = false;
}

TSubclassOf<AActor> AAlchemyCollection::GetChosenActorClass(TSubclassOf<AActor> ActorClass, FRandomStream& RandomStream)
{
    if (!ActorClass)
    {
        return nullptr;
    }
    int32 MaxDepthCheck = 100;

    TSubclassOf<AActor> ChosenClass = ActorClass;
    // Get default object (CDO) to access properties
    AAlchemyReagent* ReagentCDO = Cast<AAlchemyReagent>(ChosenClass->GetDefaultObject());
    while (ReagentCDO && ReagentCDO->SpawnCollection)
    {
        AAlchemyCollection* CollectionCDO = ReagentCDO->SpawnCollection->GetDefaultObject<AAlchemyCollection>();
        TSubclassOf<AActor> NextClass = CollectionCDO ? CollectionCDO->GetRandomActorClass(RandomStream) : nullptr;
        if (NextClass)
        {
            ChosenClass = NextClass;
            ReagentCDO = Cast<AAlchemyReagent>(ChosenClass->GetDefaultObject());
        }
        else
        {
            break;
        }
        if (--MaxDepthCheck <= 0)
        {
            break;
        }
    }

    return ChosenClass;
}

TSubclassOf<AActor> AAlchemyCollection::GetChosenActorClass(FRandomStream& RandomStream)
{
    TSubclassOf<AActor> ChosenClass = GetRandomActorClass(RandomStream);
    return GetChosenActorClass(ChosenClass, RandomStream);
}

void AAlchemyCollection::BeginPlay()
{
    Super::BeginPlay();
    ReplaceAttempts = 0;
    ReplaceThisActorWithOneFromActorEntries();
}

void AAlchemyCollection::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!SuccessfullyReplaced)
    {
        ReplaceThisActorWithOneFromActorEntries();
        if (ReplaceAttempts > 5)
        {
            // We have nothing to replace ourselves with
            Destroy();
        }
    }
}

void AAlchemyCollection::ReplaceThisActorWithOneFromActorEntries()
{
    ++ReplaceAttempts;
    TSubclassOf<AActor> ReplacementClass = GetRandomActorClass(AAlchemist::RandomStream);

    if (ReplacementClass && ReplacementClass != GetClass())
    {
        FTransform SpawnTransform = GetActorTransform();

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* Replacement = GetWorld()->SpawnActor<AActor>(ReplacementClass, SpawnTransform, SpawnParams);
        if (Replacement)
        {
            Destroy();
        }
    }
}

TSubclassOf<AActor> AAlchemyCollection::GetRandomActorClass(FRandomStream& RandomStream)
{
    float TotalWeight = 0.f;
    for (const FAlchemyActorEntry& Entry : ActorEntries)
    {
        if (Entry.ActorClass && Entry.Weight > 0.f)
        {
            TotalWeight += Entry.Weight;
        }
    }

    if (TotalWeight <= 0.f)
    {
        return nullptr;
    }

    float Choice = RandomStream.FRandRange(0.f, TotalWeight);
    float RunningWeight = 0.f;

    for (const FAlchemyActorEntry& Entry : ActorEntries)
    {
        if (Entry.ActorClass && Entry.Weight > 0.f)
        {
            RunningWeight += Entry.Weight;
            if (Choice <= RunningWeight)
            {
                return Entry.ActorClass;
            }
        }
    }

    return nullptr; // fallback (shouldn't happen unless all weights are 0.0)
}
