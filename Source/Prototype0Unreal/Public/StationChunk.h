// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelChunk.h"
#include "StationChunk.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AStationChunk : public ALevelChunk
{
	GENERATED_BODY()

public:

	bool isEnterable();
	bool isExitable();

protected:
	bool exitable;

	
};
