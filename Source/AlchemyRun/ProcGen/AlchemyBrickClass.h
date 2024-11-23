// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlchemyBrickClass.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemyBrickClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlchemyBrickClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Size of the brick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
	FVector Size = FVector(100.f, 100.f, 100.f);

	// Increment for random rotation in degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
	float RandomRotationIncrement = 0.f;

	// Number of increments rotated (calculated)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Brick")
	int32 RotationIncrementCount = 0;

	// Function to initialize the brick (can be overridden in Blueprints)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Brick")
	void InitialSpawn();

	// Rotates the brick randomly around the Z-axis
	UFUNCTION(BlueprintCallable, Category = "Brick")
	void RotateRandomly();

private:
	// Helper function to calculate random rotation increment count
	int32 CalculateRandomIncrementCount() const;
};
