// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "PressableActor.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API APressableActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	virtual void OnInteract(AMyCharacter* player);
	
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyOnPressed();
};
