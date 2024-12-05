// Copyright Alchemy Wolfe.  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AlchemyRunGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYRUN_API UAlchemyRunGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    virtual void Init() override;
};
