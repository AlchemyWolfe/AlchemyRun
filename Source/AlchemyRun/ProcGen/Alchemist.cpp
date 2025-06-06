// Copyright Alchemy Wolfe. All Rights Reserved.

#include "Alchemist.h"
#include "AlchemyRecipeComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AAlchemist::AAlchemist()
{
    PrimaryActorTick.bCanEverTick = false;
    
    FillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FillBox"));
    FillBox->SetupAttachment(RootComponent);

    GenerationSeed = 12345;
    AnimationDuration = 0.5f;
    SpawnDelay = 0.005f;
}

void AAlchemist::BeginPlay()
{
    Super::BeginPlay();
    GenerateLevel();
}

void AAlchemist::GenerateLevel()
{
    ClearGeneratedActors();

    RandomStream.Initialize(GenerationSeed);
    GenerateBreadthFirst();
}

void AAlchemist::ClearGeneratedActors()
{
}

void AAlchemist::ProcessRecipe(UAlchemyRecipeComponent* Recipe)
{
    if (!Recipe)
    {
        return;
    }

    Recipe->GenerateReagents(RandomStream.GetUnsignedInt(), this, FillBox);

    for (AActor* Actor : Recipe->GeneratedActors)
    {
        PlaySpawnAnimation(Actor);
    }
}

void AAlchemist::PlaySpawnAnimation(AActor* Actor)
{
    if (!Actor)
    {
        return;
    }

    Actor->SetActorScale3D(FVector(0.01f));
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Actor]()
    {
        Actor->SetActorScale3D(FVector(1.1f));
        FTimerHandle InnerTimerHandle;
        Actor->GetWorld()->GetTimerManager().SetTimer(InnerTimerHandle, [Actor]()
        {
            Actor->SetActorScale3D(FVector(1.0f));
        }, 0.1f, false);
    }, AnimationDuration, false);
}

void AAlchemist::GenerateBreadthFirst()
{
    TQueue<UAlchemyRecipeComponent*> RecipeQueue;

    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

    for (AActor* Actor : AllActors)
    {
        if (UAlchemyRecipeComponent* Recipe = Actor->FindComponentByClass<UAlchemyRecipeComponent>())
        {
            RecipeQueue.Enqueue(Recipe);
        }
    }

    while (!RecipeQueue.IsEmpty())
    {
        UAlchemyRecipeComponent* CurrentRecipe = nullptr;
        RecipeQueue.Dequeue(CurrentRecipe);

        if (CurrentRecipe)
        {
            ProcessRecipe(CurrentRecipe);

            for (AActor* GeneratedActor : CurrentRecipe->GeneratedActors)
            {
                if (UAlchemyRecipeComponent* ChildRecipe = GeneratedActor->FindComponentByClass<UAlchemyRecipeComponent>())
                {
                    RecipeQueue.Enqueue(ChildRecipe);
                }
            }
        }
    }
}
