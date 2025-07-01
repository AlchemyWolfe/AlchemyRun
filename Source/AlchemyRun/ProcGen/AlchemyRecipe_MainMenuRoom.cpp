// Copyright Alchemy Wolfe.  All Rights Reserved.


#include "AlchemyRecipe_MainMenuRoom.h"


void UAlchemyRecipe_MainMenuRoom::GenerateStructureReagentsImpl()
{
	FillRing(WallActor, CornerActor, 0, 0, GridMaxX, GridMaxY, FloorTileSize.Z);
	FillRing(FloorActor, FloorActor, 0, 0, GridMaxX, GridMaxY, FloorTileSize.Z);
	FillRing(StairsActor, FloorActor, 1, 1, GridMaxX-1, GridMaxY-1, FloorTileSize.Z);
	FillGrid(FloorActor, 1, 1, GridMaxX-1, GridMaxY-1, 0);
	int MidX = GridMaxX / 2;
	int MidY = GridMaxY / 2;
	AddDoorwayInWall(0, MidY, true);
	AddDoorwayInWall(GridMaxX, MidY, true);
	AddDoorwayInWall(MidX, 0, true);
	AddDoorwayInWall(MidX, GridMaxY, true);
}

void UAlchemyRecipe_MainMenuRoom::GenerateRequiredReagentsImpl()
{
	Super::GenerateRequiredReagentsImpl();
}

void UAlchemyRecipe_MainMenuRoom::GenerateDecorationReagentsImpl()
{
	Super::GenerateDecorationReagentsImpl();
}
