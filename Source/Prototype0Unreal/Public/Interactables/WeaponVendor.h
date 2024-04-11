// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Vendor.h"
#include "WeaponVendor.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AWeaponVendor : public AVendor
{
	GENERATED_BODY()
	
public:
	AWeaponVendor();

	UPROPERTY(BlueprintReadOnly)
	bool HasPurchased;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeapon> AvailableWeapon;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText getInfoText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteract(AMyCharacter* player) override;

	void ApplyWeapon(AMyCharacter* player);

	virtual bool CanPlayerInteract(AMyCharacter* player) override;
};
