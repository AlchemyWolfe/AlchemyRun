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

private:
	float GridSize;
	FVector Origin;
};
