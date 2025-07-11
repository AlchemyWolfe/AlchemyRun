// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AlchemyCollection.h"
#include "GameFramework/Actor.h"
#include "AlchemyReagent.generated.h"

UCLASS()
class ALCHEMYRUN_API AAlchemyReagent : public AActor
{
	GENERATED_BODY()
	friend class AAlchemist;
	
public:	
	// Sets default values for this actor's properties
	AAlchemyReagent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	TSubclassOf<AAlchemyCollection> SpawnCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContentAlchemy")
	int32 Seed = 0;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	FRandomStream RandomStream;
};

