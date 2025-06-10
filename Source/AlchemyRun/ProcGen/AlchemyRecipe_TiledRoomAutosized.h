// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyRecipeComponent.h"
#include "AlchemyRecipe_TiledRoomAutosized.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ALCHEMYRUN_API UAlchemyRecipe_TiledRoomAutosized : public UAlchemyRecipeComponent
{
	GENERATED_BODY()

public:
	UAlchemyRecipe_TiledRoomAutosized();

	// Meshes to use
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	UStaticMesh* FloorTile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	UStaticMesh* WallTile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	float WallOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	UStaticMesh* CornerTile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	FVector2D CornerOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	UStaticMesh* DoorTile;

	// Floor tile spans how many grid cells?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tiles")
	int32 FloorTileGridSpan;

	// Height of the wall in grid units
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tiles")
	float WallHeight;

protected:
	virtual void GenerateReagentsImpl(AActor* Parent, UBoxComponent* FillBox) override;

private:
	float GridSize;
	FVector Origin;
};
