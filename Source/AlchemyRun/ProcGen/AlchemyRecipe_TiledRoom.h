// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyRecipeComponent.h"
#include "AlchemyRecipe_TiledRoom.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ALCHEMYRUN_API UAlchemyRecipe_TiledRoom : public UAlchemyRecipeComponent
{
	GENERATED_BODY()

public:
	UAlchemyRecipe_TiledRoom();

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> FloorActor;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> WallActor;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> CornerActor;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> DoorwayActor;

	virtual void InitializeRecipe(int32 Seed, const AActor* Parent, UBoxComponent* FillBox) override;
	
protected:
	virtual void GenerateStructureReagentsImpl() override;
	virtual void GenerateRequiredReagentsImpl() override;
	virtual void GenerateDecorationReagentsImpl() override;
	void FillGrid(const TSubclassOf<AActor>& ActorClass, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z);
	void FillRing(const TSubclassOf<AActor>& WallActorClass, const TSubclassOf<AActor>& CornerActorClass, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z);
	AActor* AddDoorwayInWall(int X, int Y, bool DoorExists = false);
	AActor* RemoveDoorwayInWall(int X, int Y);
	void FillNorthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z);
	void FillSouthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z);
	void FillEastWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z);
	void FillWestWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z);
	void SpawnNWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	void SpawnNECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	void SpawnSWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	void SpawnSECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	
	inline FVector GetGridLocation(int32 x, int32 y, float z) const
	{
		return GridOrigin + FVector(x * GridSize, y * GridSize, z);
	}
	
	inline FVector GetGridLocationCenter(int32 x, int32 y, float z) const
	{
		return GridOrigin + FVector(x * GridSize + GridSize*0.5f, y * GridSize + GridSize*0.5f, z);
	}
	
protected:
	bool GridInitialized;
	float GridSize;
	int32 GridMaxX;
	int32 GridMaxY;
	FVector FloorTileSize;
	FVector GridOrigin;	// Lower left in X,Y
};
