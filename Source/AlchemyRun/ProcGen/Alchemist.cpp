// Copyright Alchemy Wolfe. All Rights Reserved.

#include "Alchemist.h"
#include "AlchemyRecipeComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AAlchemist::AAlchemist()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    FillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FillBox"));
    FillBox->SetupAttachment(RootComponent);
    FillBox->SetBoxExtent(FVector(500.f * 3.5, 500.f * 3.5, 500.f));
    FillBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    GenerationSeed = 12345;
    AnimationDuration = 0.5f;
    SpawnDelay = 0.005f;
}

void AAlchemist::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AAlchemist::BeginPlay()
{
    Super::BeginPlay();
    GenerateLevel();
}

void AAlchemist::RegenerateLevel()
{
    GenerateLevel();
}

void AAlchemist::ClearLevel()
{
    ClearGeneratedActors();
}

void AAlchemist::EnqueueTopLevelRecipies()
{
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

    for (AActor* Actor : AllActors)
    {
        if (UAlchemyRecipeComponent* Recipe = Actor->FindComponentByClass<UAlchemyRecipeComponent>())
        {
            RecipeQueue.Enqueue(Recipe);
        }
    }
}

void AAlchemist::GenerateLevel()
{
    ClearGeneratedActors();

    RandomStream.Initialize(GenerationSeed);
    GenerateBreadthFirst();
}

void AAlchemist::ClearGeneratedActors()
{
    EnqueueTopLevelRecipies();
    while (!RecipeQueue.IsEmpty())
    {
        UAlchemyRecipeComponent* CurrentRecipe = nullptr;
        RecipeQueue.Dequeue(CurrentRecipe);

        if (CurrentRecipe)
        {
            CurrentRecipe->ClearGeneratedActors();
        }
    }
}

void AAlchemist::ProcessRecipe(UAlchemyRecipeComponent* Recipe)
{
    if (!Recipe)
    {
        return;
    }

    Recipe->InitializeRecipe(RandomStream.GetUnsignedInt(), this, FillBox);
    Recipe->GenerateStructureReagents();
    Recipe->GenerateRequiredReagents();
    Recipe->GenerateDecorationReagents();

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
    EnqueueTopLevelRecipies();
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
