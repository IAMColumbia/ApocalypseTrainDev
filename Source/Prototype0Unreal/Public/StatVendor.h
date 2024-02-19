// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vendor.h"
#include "StatVendor.generated.h"

enum class EUpgradeType : uint8;

UCLASS()
class PROTOTYPE0UNREAL_API AStatVendor : public AVendor
{
	GENERATED_BODY()
	

protected:
	
	virtual void OnInteract(AMyCharacter* player) override;

public:
	// Called every frame

	UPROPERTY(EditDefaultsOnly)
	EUpgradeType upgradeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int UpgradeAmount;

	UFUNCTION(BlueprintImplementableEvent)
	void PurchasedUpgrade(const FText& textToDisplay, FVector color);

	FText getUpgradeText(float value, EUpgradeType type);
};

