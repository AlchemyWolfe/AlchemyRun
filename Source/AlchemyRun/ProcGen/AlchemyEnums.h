// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EReagentPlacementLocation : uint8
{
	Anywhere     UMETA(DisplayName = "Anywhere"),
	Center       UMETA(DisplayName = "Center"),
	AroundCenter UMETA(DisplayName = "AroundCenter"),
	AlongWalls	 UMETA(DisplayName = "AlongWalls"),
	AnyWall      UMETA(DisplayName = "Any Wall"),
	NorthWall    UMETA(DisplayName = "North Wall"),
	SouthWall    UMETA(DisplayName = "South Wall"),
	EastWall     UMETA(DisplayName = "East Wall"),
	WestWall     UMETA(DisplayName = "West Wall"),
	AnyCorner    UMETA(DisplayName = "Any Corner"),
	NWCorner     UMETA(DisplayName = "NW Corner"),
	NECorner     UMETA(DisplayName = "NE Corner"),
	SWCorner     UMETA(DisplayName = "SW Corner"),
	SECorner     UMETA(DisplayName = "SE Corner")
};

UENUM(BlueprintType)
enum class EReagentPlacementFacing : uint8
{
	Any            UMETA(DisplayName = "Any"),
	TowardsCenter  UMETA(DisplayName = "Towards Center"),
	AwayFromCenter UMETA(DisplayName = "Away From Center"),
	North          UMETA(DisplayName = "North"),
	South          UMETA(DisplayName = "South"),
	East           UMETA(DisplayName = "East"),
	West           UMETA(DisplayName = "West"),
	NW		       UMETA(DisplayName = "NW"),
	NE			   UMETA(DisplayName = "NE"),
	SW			   UMETA(DisplayName = "SW"),
	SE			   UMETA(DisplayName = "SE"),
	TowardsWall    UMETA(DisplayName = "Towards Wall"),
	AwayFromWall   UMETA(DisplayName = "Away From Wall"),
	TowardsCorner  UMETA(DisplayName = "Towards Corner"),
	AwayFromCorner UMETA(DisplayName = "Away From Corner")
};

UENUM(BlueprintType)
enum class EReagentFillBoxBehavior : uint8
{
	None        UMETA(DisplayName = "None"),
	Footprint   UMETA(DisplayName = "Footprint"),
	StretchNS   UMETA(DisplayName = "StretchNS"),
	StretchEW   UMETA(DisplayName = "StretchEW")
};

