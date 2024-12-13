// Copyright Alchemy Wolfe.  All Rights Reserved.

#include "AlchemyCauldron.h"
#include "AlchemyRecipe.h"
#include "AlchemyReagent.h"

void UAlchemyRecipe::ClearReagents()
{
	for (AAlchemyReagent* Reagent : CreatedReagents)
	{
		if (Reagent)
		{
			Reagent->Destroy();
		}
	}
	CreatedReagents.Empty();
}

AAlchemyReagent* UAlchemyRecipe::SpawnReagent(UAlchemyReagentEntry* ReagentEntry, float RotationIncrement, FVector RelativeLocation, FRotator RelativeRotation)
{
	AAlchemyReagent* Reagent = ReagentEntry->SpawnActor((AActor*)ParentReagent, RotationIncrement, RelativeLocation, RelativeRotation);
	if (Reagent)
	{
		CreatedReagents.Add(Reagent);
	}
	return Reagent;
}

AAlchemyReagent* UAlchemyRecipe::GenerateReagents(int32 Seed, AAlchemyReagent* Parent, UBoxComponent* FillBox, const FString& CauldronName)
{
	if (!Parent || !FillBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parent or FillBox is null!"));
		return nullptr;
	}

	ClearReagents();
	RandomStream.Initialize(Seed);

	AAlchemyCauldron* Cauldron = nullptr;

	// Check if a name is provided to create a cauldron
	if (!CauldronName.IsEmpty())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Parent;
		SpawnParams.Name = *CauldronName;

		// Spawn the cauldron
		Cauldron = GetWorld()->SpawnActor<AAlchemyCauldron>(
			AAlchemyCauldron::StaticClass(),
			Parent->GetActorLocation(),    // Position (initially matches Parent)
			Parent->GetActorRotation(),    // Rotation (initially matches Parent)
			SpawnParams
		);

		if (Cauldron)
		{
			// Attach to the parent
			Cauldron->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);

			// Set local position and rotation to zero
			Cauldron->SetActorRelativeLocation(FVector::ZeroVector);
			Cauldron->SetActorRelativeRotation(FRotator::ZeroRotator);
			Parent = Cauldron;
		}
	}

	ParentReagent = Parent;
	return Parent;
}

