// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyReagent_ActorSpawner.h"
#include "Alchemist.h"
#include "RenderGraphUtils.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAlchemyReagent_ActorSpawner::AAlchemyReagent_ActorSpawner()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false);

    // Create and attach FillBox
    FillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FillBox"));
    //RootComponent = FillBox;
    FillBox->SetBoxExtent(FVector(100.f)); // default size
    FillBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // or as needed
    FillBox->SetVisibility(true);
}

void AAlchemyReagent_ActorSpawner::StartSpawning(FRandomStream& NewRandomStream, UBoxComponent* NewFillBox)
{
    RandomStream = NewRandomStream;
    FillBox = NewFillBox;
    SetActorTickEnabled(true);
}

void AAlchemyReagent_ActorSpawner::BeginPlay()
{
    Super::BeginPlay();
    const FVector Origin = FillBox->GetComponentLocation();
    const FVector Extent = FillBox->GetScaledBoxExtent();
    
    // Only spawn at start if we want to.
    SetActorTickEnabled(SpawnOnStart);
}

void AAlchemyReagent_ActorSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!IsRootComponentCollisionRegistered())
    {
        // Skip this tick because we are not ready
        return;
    }
    if (!FillBox || !SpawnCollection || TotalSpawned >= SpawnCount)
    {
        SetActorTickEnabled(false);
        return;
    }

    // Spawn an actor
    const FVector Origin = FillBox->GetComponentLocation();
    const FVector Extent = FillBox->GetScaledBoxExtent();

    AAlchemyCollection* CollectionCDO = SpawnCollection->GetDefaultObject<AAlchemyCollection>();
    if (!CollectionCDO)
    {
        return;
    }
    TSubclassOf<AActor> ActorClass = CollectionCDO->GetChosenActorClass(RandomStream);
    if (!ActorClass)
    {
        return;
    }

    FVector SpawnLocation;
    FRotator SpawnRotation;

    if (!AAlchemist::GetDesiredSpawnLocation(RandomStream, SpawnLocation, SpawnRotation, Extent, PlacementBuffer, PlacementLocation, PlacementFacing, -1))
    {
        SpawnLocation = FVector(0,0,0);
        SpawnRotation = FRotator(0, 0, 0);
    }

    AActor *SpawnedActor = SpawnActor(ActorClass, SpawnLocation, SpawnRotation);
    if (SpawnedActor != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("AAlchemyReagent_ActorSpawner::Tick SPAWN!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AAlchemyReagent_ActorSpawner::Tick no spawn"));
    }
}

void AAlchemyReagent_ActorSpawner::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    ReadyToSpawn = true;
}

AActor* AAlchemyReagent_ActorSpawner::SpawnActor(const TSubclassOf<AActor>& ActorClass, const FVector& Location,
                                                 const FRotator& Rotation)
{
    if (!ActorClass) return  nullptr;

    // Drop to floor if possible
    FVector TraceStart = Location + FVector(0, 0, PlacementBuffer);
    FVector TraceEnd = Location - FVector(0, 0, 1000);
    FHitResult Hit;
    //FCollisionQueryParams Params;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(LineTrace), true);
    Params.bReturnPhysicalMaterial = false;
    Params.AddIgnoredActor(this);
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);
    FVector SpawnLocation = bHit ? Hit.Location : Location;
    //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.0f, 0, 1.0f);
    if (!bHit)
    {
        return nullptr;
    }
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnLocation, Rotation, SpawnParams);

    if (SpawnedActor)
    {
        ++TotalSpawned;
        
        // Track destruction
        SpawnedActor->OnDestroyed.AddDynamic(this, &AAlchemyReagent_ActorSpawner::OnSpawnedActorDestroyed);

        // Optional: store for reference
        SpawnedActors.Add(SpawnedActor);
    }

    return SpawnedActor;
}

void AAlchemyReagent_ActorSpawner::OnSpawnedActorDestroyed(AActor* DestroyedActor)
{
    SpawnedActors.Remove(DestroyedActor);
}
