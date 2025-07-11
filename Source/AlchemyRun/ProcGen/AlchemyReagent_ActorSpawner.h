// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h" // Include the necessary header
#include "AlchemyCollection.h"
#include "AlchemyRecipeComponent.h"
#include "AlchemyEnums.h"
#include "AlchemyReagent_ActorSpawner.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemyReagent_ActorSpawner : public AActor
{
	GENERATED_BODY()

public:
	AAlchemyReagent_ActorSpawner();

	UFUNCTION(BlueprintCallable, Category = "ContentAlchemy")
	void StartSpawning(FRandomStream& RandomStream, UBoxComponent* FillBox);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
	// The actor collection to spawn from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TSubclassOf<AAlchemyCollection> SpawnCollection;

	// Where to spawn inside of
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ContentAlchemy")
	UBoxComponent* FillBox;

	// Number of actors to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	int32 SpawnCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	EReagentPlacementLocation PlacementLocation = EReagentPlacementLocation::Anywhere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	EReagentPlacementFacing PlacementFacing = EReagentPlacementFacing::Any;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	float PlacementBuffer = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	EReagentFillBoxBehavior FillBoxBehavior = EReagentFillBoxBehavior::None;

	// Whether to spawn automatically on BeginPlay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	bool SpawnOnStart = false;

protected:
	UAlchemyRecipeComponent* ParentRecipe;
	FRandomStream RandomStream;
	int32 TotalSpawned = 0;
	bool ReadyToSpawn = false;
	
	FRotator GetFacingRotation(const FVector& FromLocation);

	AActor* SpawnActor(const TSubclassOf<AActor>& ActorClass, const FVector& Location, const FRotator& Rotation);

	UFUNCTION()
	void OnSpawnedActorDestroyed(AActor* DestroyedActor);

	UPROPERTY()
	TArray<AActor*> SpawnedActors;
};