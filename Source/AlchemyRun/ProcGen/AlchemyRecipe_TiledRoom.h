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

	// Floor tile spans how many grid cells?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	int32 FloorTileGridSpan;

	// Static Meshes and/or Actors to use.
	UPROPERTY(EditAnywhere, Category="Tiles")
	UStaticMesh* FloorSM;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> FloorActor;

	UPROPERTY(EditAnywhere, Category="Tiles")
	UStaticMesh* WallSM;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> WallActor;

	UPROPERTY(EditAnywhere, Category="Tiles")
	UStaticMesh* CornerSM;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> CornerActor;

	UPROPERTY(EditAnywhere, Category="Tiles")
	UStaticMesh* DoorSM;

	UPROPERTY(EditAnywhere, Category="Tiles")
	TSubclassOf<AActor> DoorActor;
	
protected:
	virtual void GenerateReagentsImpl(AActor* Parent, UBoxComponent* FillBox) override;
	void InitializeGridValues(const AActor* Parent, const UBoxComponent* FillBox);
	void FillGrid(UStaticMesh* Mesh, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z, int32 Step = 1);
	void FillGrid(const TSubclassOf<AActor>& ActorClass, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, float Z, int32 Step = 1);
	void FillNorthWall(UStaticMesh* Mesh, int32 X, int32 MinY, int32 MaxY, float Z);
	void FillNorthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z);
	void FillSouthWall(UStaticMesh* Mesh, int32 X, int32 MinY, int32 MaxY, float Z);
	void FillSouthWall(const TSubclassOf<AActor>& ActorClass, int32 X, int32 MinY, int32 MaxY, float Z);
	void FillEastWall(UStaticMesh* Mesh, int32 Y, int32 MinX, int32 MaxX, float Z);
	void FillEastWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z);
	void FillWestWall(UStaticMesh* Mesh, int32 Y, int32 MinX, int32 MaxX, float Z);
	void FillWestWall(const TSubclassOf<AActor>& ActorClass, int32 Y, int32 MinX, int32 MaxX, float Z);
	void SpawnNWCorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z);
	void SpawnNWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	void SpawnNECorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z);
	void SpawnNECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	void SpawnSWCorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z);
	void SpawnSWCorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	void SpawnSECorner(UStaticMesh* Mesh, int32 X, int32 Y, float Z);
	void SpawnSECorner(const TSubclassOf<AActor>& ActorClass, int32 X, int32 Y, float Z);
	
	inline FVector GetGridLocation(int32 x, int32 y, float z) const
	{
		return FillOrigin + FVector(x * GridSize, y * GridSize, z);
	}
	
private:
	bool GridInitialized;
	float GridSize;
	int32 GridMaxX;
	int32 GridMaxY;
	FVector Origin;
	USceneComponent* AttachParent;
	FVector FillExtent;
	FVector FillOrigin;
	FVector TileSize;
};
