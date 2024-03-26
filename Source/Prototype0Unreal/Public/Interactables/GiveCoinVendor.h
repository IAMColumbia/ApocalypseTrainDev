// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vendor.h"
#include "GiveCoinVendor.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AGiveCoinVendor : public AVendor
{
	GENERATED_BODY()

protected:

	virtual void CheckForInteractPressed() override;

	virtual void OnInteract(AMyCharacter* player) override;
	
};
