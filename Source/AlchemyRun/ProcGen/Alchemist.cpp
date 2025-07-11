// Copyright Alchemy Wolfe. All Rights Reserved.

#include "Alchemist.h"
#include "AlchemyRecipeComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

FRandomStream AAlchemist::RandomStream;

AAlchemist::AAlchemist()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Create and attach FillBox
    FillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FillBox"));
    FillBox->SetupAttachment(RootComponent);
    FillBox->SetBoxExtent(FVector(500.f * 3.5, 500.f * 3.5, 500.f));
    FillBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // or as needed
    FillBox->SetVisibility(true);
    
    AnimationDuration = 0.5f;
    SpawnDelay = 0.005f;
}

AActor* AAlchemist::SpawnReagent(UStaticMesh* Mesh, const FString& NameTag, const FVector& Location,
    const FRotator& Rotation, UAlchemyRecipeComponent* Recipe)
{
    if (!Mesh || !Recipe) return nullptr;
    UWorld* World = Recipe->AttachParent->GetWorld();
    if (!World) return nullptr;

    FTransform ParentTransform = Recipe->AttachParent->GetComponentTransform();
    FVector WorldLocation = ParentTransform.TransformPosition(Location);
    FRotator WorldRotation = ParentTransform.TransformRotation(Rotation.Quaternion()).Rotator();

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
    // Spawn empty actor
    AActor* SpawnedActor = World->SpawnActor<AActor>(AActor::StaticClass(), WorldLocation, WorldRotation, Params);
    if (!SpawnedActor) return nullptr;

    SpawnedActor->Tags.Add(FName(NameTag));

    // Create and attach mesh component
    UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(SpawnedActor);
    MeshComp->SetStaticMesh(Mesh);
    MeshComp->SetupAttachment(nullptr); // No parent yet
    MeshComp->SetWorldLocation(WorldLocation);
    MeshComp->SetWorldRotation(WorldRotation);
    MeshComp->RegisterComponent();

    // Set as root BEFORE attaching actor
    SpawnedActor->SetRootComponent(MeshComp);

    // Now attach to parent
    SpawnedActor->AttachToComponent(Recipe->AttachParent, FAttachmentTransformRules::KeepWorldTransform);

    // Track the spawned actor
    Recipe->GeneratedActors.Add(SpawnedActor);

    return SpawnedActor;
}

AActor* AAlchemist::SpawnReagent(const TSubclassOf<AActor>& ActorClass, const FString& NameTag, const FVector& Location,
    const FRotator& Rotation, UAlchemyRecipeComponent* Recipe, bool LocationIsLocal)
{
    if (!ActorClass || !Recipe || !Recipe->AttachParent) return nullptr;
    UWorld* World = Recipe->AttachParent->GetWorld();
    if (!World) return nullptr;

    FTransform ParentTransform = Recipe->AttachParent->GetComponentTransform();
    FVector WorldLocation = LocationIsLocal ? ParentTransform.TransformPosition(Location) : Location;
    FRotator WorldRotation = LocationIsLocal ? ParentTransform.TransformRotation(Rotation.Quaternion()).Rotator() : Rotation;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, WorldLocation, WorldRotation, Params);
    if (!SpawnedActor) return nullptr;
    SpawnedActor->Tags.Add(FName(NameTag));
    
    SpawnedActor->AttachToComponent(Recipe->AttachParent, FAttachmentTransformRules::KeepWorldTransform);
    Recipe->GeneratedActors.Add(SpawnedActor);
    
    // Call Blueprint event: OnReagentSpawned(int32)
    UFunction* InitFunc = SpawnedActor->FindFunction(FName("OnReagentSpawned"));
    if (InitFunc)
    {
        struct
        {
            int32 Seed;
        } SpawnedParams;
        SpawnedParams.Seed = static_cast<int32>(Recipe->RandomStream.GetUnsignedInt());

        SpawnedActor->ProcessEvent(InitFunc, &SpawnedParams);
    }
    
    return SpawnedActor;
}

AActor* AAlchemist::SpawnReagentInPlaceOf(UAlchemyRecipeComponent* Recipe, AActor* OldReagent,
    const FString& NewNameTag, const TSubclassOf<AActor>& NewActorClass)
{
    if (!NewActorClass || !OldReagent) return nullptr;

    FVector Location = OldReagent->GetActorLocation();
    FRotator Rotation = OldReagent->GetActorRotation();

    // Destroy the old actor and remove it from the list
    if (Recipe)
    {
        auto i = Recipe->GeneratedActors.Find(OldReagent);
        Recipe->GeneratedActors.RemoveAt(i);
    }
    OldReagent->Destroy();

    // Spawn and return the replacement
    return SpawnReagent(NewActorClass, NewNameTag, Location, Rotation, Recipe, false);
}

bool AAlchemist::GetDesiredSpawnLocation(FRandomStream& Random, FVector& OutLocalLocation, FRotator& OutLocalRotation,
    const FVector& Extent, float Buffer, EReagentPlacementLocation Location, EReagentPlacementFacing Facing, float Position)
{
    // Determine base location
    FVector LocalPos = FVector(0,0,0);

    // Process our random locations
    switch (Location)
    {
    case EReagentPlacementLocation::AnyWall:
    {
        int32 WallIndex = Random.RandRange(0, 3);
        Location = static_cast<EReagentPlacementLocation>(static_cast<int32>(EReagentPlacementLocation::NorthWall) + WallIndex);
        break;
    }

    case EReagentPlacementLocation::AnyCorner:
    {
        int32 CornerIndex = Random.RandRange(0, 3);
        Location = static_cast<EReagentPlacementLocation>(static_cast<int32>(EReagentPlacementLocation::NWCorner) + CornerIndex);
        break;
    }
        
    default:
        break;
    }
    if (Position <= 0.0f || Position > 1.0f)
    {
        // I expect to use -1.0 as Make It Random.
        Position = Random.FRandRange(0.0f, 1.0f);
    }

    switch (Location)
    {
    case EReagentPlacementLocation::Center:
        break;

    case EReagentPlacementLocation::Anywhere:
        LocalPos = FVector(
        Random.FRandRange(-Extent.X + Buffer, Extent.X - Buffer),
        Random.FRandRange(-Extent.Y + Buffer, Extent.Y - Buffer),
        0.0f);
        break;

    case EReagentPlacementLocation::AroundCenter:
    {
        float Angle = Position * 2.f * PI;
        LocalPos = FVector(
        Buffer * FMath::Cos(Angle),
        Buffer * FMath::Sin(Angle),
        0.0f);
        break;
    }

    case EReagentPlacementLocation::AlongWalls:
    {
        float Width = Extent.X * 2.f;
        float Height = Extent.Y * 2.f;
        float Perimeter = 2.f * (Width + Height);
        float Distance = Position * Perimeter;

        // Walking clockwise: North -> East -> South -> West
        if (Distance < Width)
        {
            // North wall: left to right (West to East)
            float LocalX = -Extent.X + Distance;
            LocalPos = FVector(LocalX, Extent.Y - Buffer, 0.0f);
        }
        else if (Distance < Width + Height)
        {
            // East wall: top to bottom (North to South)
            float LocalY = Extent.Y - (Distance - Width);
            LocalPos = FVector(Extent.X - Buffer, LocalY, 0.0f);
        }
        else if (Distance < 2 * Width + Height)
        {
            // South wall: right to left (East to West)
            float LocalX = Extent.X - (Distance - (Width + Height));
            LocalPos = FVector(LocalX, -Extent.Y + Buffer, 0.0f);
        }
        else
        {
            // West wall: bottom to top (South to North)
            float LocalY = -Extent.Y + (Distance - (2 * Width + Height));
            LocalPos = FVector(-Extent.X + Buffer, LocalY, 0.0f);
        }
        break;
    }
    
    case EReagentPlacementLocation::NorthWall:
        LocalPos = FVector(
        Extent.X - Buffer,
        FMath::Lerp(-Extent.Y + Buffer, Extent.Y - Buffer, Position),
        0.0f);
        break;

    case EReagentPlacementLocation::SouthWall:
        LocalPos = FVector(
        -Extent.X + Buffer,
        FMath::Lerp(-Extent.Y + Buffer, Extent.Y - Buffer, Position),
        0.0f);
        break;

    case EReagentPlacementLocation::EastWall:
        LocalPos = FVector(
        FMath::Lerp(-Extent.X + Buffer, Extent.X - Buffer, Position),
        Extent.Y - Buffer,
        0.0f);
        break;

    case EReagentPlacementLocation::WestWall:
        LocalPos = FVector(
        FMath::Lerp(-Extent.X + Buffer, Extent.X - Buffer, Position),
        -Extent.Y + Buffer,
        0.0f);
        break;

    case EReagentPlacementLocation::NWCorner:
        LocalPos = FVector(
        -Extent.X + Buffer,
        Extent.Y - Buffer,
        0.0f);
        break;

    case EReagentPlacementLocation::NECorner:
        LocalPos = FVector(
        Extent.X - Buffer,
        Extent.Y - Buffer,
        0.0f);
        break;

    case EReagentPlacementLocation::SWCorner:
        LocalPos = FVector(
        -Extent.X + Buffer,
        -Extent.Y + Buffer,
        0.0f);
        break;

    case EReagentPlacementLocation::SECorner:
        LocalPos = FVector(
            Extent.X - Buffer,
            -Extent.Y + Buffer,
            0.0f);
        break;

    default:
        break;
    }

    // Process our random facings
    if (Location == EReagentPlacementLocation::Center && (Facing == EReagentPlacementFacing::TowardsCenter || Facing == EReagentPlacementFacing::AwayFromCenter))
    {
        // If we're in the center, we can't use the center as a facing reference.
        Facing = EReagentPlacementFacing::Any;
    }
    switch (Facing)
    {
    case EReagentPlacementFacing::TowardsWall:
        {
            if (FMath::Abs(LocalPos.X) > FMath::Abs(LocalPos.Y))
            {
                // Closer to East/West wall
                Facing = (LocalPos.X < 0) ? EReagentPlacementFacing::East : EReagentPlacementFacing::West;
            }
            else
            {
                // Closer to North/South wall
                Facing = (LocalPos.Y < 0) ? EReagentPlacementFacing::North : EReagentPlacementFacing::South;
            }
            break;
        }

    case EReagentPlacementFacing::AwayFromWall:
        {
            if (FMath::Abs(LocalPos.X) > FMath::Abs(LocalPos.Y))
            {
                // Closer to East/West wall
                Facing = (LocalPos.X < 0) ? EReagentPlacementFacing::West : EReagentPlacementFacing::East;
            }
            else
            {
                // Closer to North/South wall
                Facing = (LocalPos.Y < 0) ? EReagentPlacementFacing::South : EReagentPlacementFacing::North;
            }
            break;
        }

    case EReagentPlacementFacing::TowardsCorner:
        {
            bool bWest = LocalPos.X < 0;
            bool bSouth = LocalPos.Y < 0;

            if (bWest && bSouth)
            {
                Facing = EReagentPlacementFacing::NE; // Facing northeast from southwest
            }
            else if (bWest && !bSouth)
            {
                Facing = EReagentPlacementFacing::SE; // Facing southeast from northwest
            }
            else if (!bWest && bSouth)
            {
                Facing = EReagentPlacementFacing::NW; // Facing northwest from southeast
            }
            else // (!bWest && !bSouth)
            {
                Facing = EReagentPlacementFacing::SW; // Facing southwest from northeast
            }
            break;
        }

    case EReagentPlacementFacing::AwayFromCorner:
        {
            bool bWest = LocalPos.X < 0;
            bool bSouth = LocalPos.Y < 0;

            if (bWest && bSouth)
            {
                Facing = EReagentPlacementFacing::SW;
            }
            else if (bWest && !bSouth)
            {
                Facing = EReagentPlacementFacing::NW;
            }
            else if (!bWest && bSouth)
            {
                Facing = EReagentPlacementFacing::SE;
            }
            else // (!bWest && !bSouth)
            {
                Facing = EReagentPlacementFacing::NE;
            }
            break;
        }

    default:
        break;
    }

    // Facing calculation
    FVector Forward;

    switch (Facing)
    {
    case EReagentPlacementFacing::North:
        Forward = FVector(1.f, 0.f, 0.f);
        break;

    case EReagentPlacementFacing::South:
        Forward = FVector(-1.f, 0.f, 0.f);
        break;

    case EReagentPlacementFacing::East:
        Forward = FVector(0.f, -1.f, 0.f);
        break;

    case EReagentPlacementFacing::West:
        Forward = FVector(0.f, 1.f, 0.f);
        break;

    case EReagentPlacementFacing::NW:
        Forward = FVector(SQRT_2_INV, SQRT_2_INV, 0.f);
        break;

    case EReagentPlacementFacing::NE:
        Forward = FVector(SQRT_2_INV, -SQRT_2_INV, 0.f);
        break;

    case EReagentPlacementFacing::SW:
        Forward = FVector(-SQRT_2_INV, SQRT_2_INV, 0.f);
        break;

    case EReagentPlacementFacing::SE:
        Forward = FVector(-SQRT_2_INV, -SQRT_2_INV, 0.f);
        break;

    case EReagentPlacementFacing::TowardsCenter:
        Forward = (-LocalPos).GetSafeNormal();
        break;

    case EReagentPlacementFacing::AwayFromCenter:
        Forward = LocalPos.GetSafeNormal();
        break;

    case EReagentPlacementFacing::Any:
    default:
        Forward = FRotator(0, Random.FRandRange(0.f, 360.f), 0).Vector();
        break;
    }

    OutLocalLocation = LocalPos;
    OutLocalRotation = Forward.Rotation();
    return true;
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

    (GenerationSeed < 0) ? RandomStream.Initialize(FPlatformTime::Cycles()): RandomStream.Initialize(GenerationSeed);
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
