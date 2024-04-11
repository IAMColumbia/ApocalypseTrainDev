// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/WeaponVendor.h"
#include "Characters/MyCharacter.h"
#include "Weapons/Weapon.h"

AWeaponVendor::AWeaponVendor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponVendor::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponVendor::OnInteract(AMyCharacter* player)
{
	if (HasPurchased) {
		return;
	}
	if (player->Carrying) {
		return;
	}
	if (!CanPlayerInteract(player)) {
		return;
	}
	if (!Dispensing) {
		if (player->Coins >= CostOfItem) {
			Dispensing = true;
			ApplyWeapon(player);
			player->LoseCoins(CostOfItem);
			HasPurchased = true;
			OnPurchase();
		}
	}
}

void AWeaponVendor::ApplyWeapon(AMyCharacter* player)
{
	AvailableWeapon = player->PickupWeapon(AvailableWeapon);
}

bool AWeaponVendor::CanPlayerInteract(AMyCharacter* player)
{
	TSubclassOf<AWeapon> curWeapon = player->GetCurrentWeapon()->GetClass();
	if (curWeapon == AvailableWeapon) {
		GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, TEXT("THEY ARE THE SAME"));
		return false;
	}
	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, TEXT("NOT SIMILARQ!!"));
	return true;
}

FText AWeaponVendor::getInfoText()
{
	return FText::Format(INVTEXT("{0}"), CostOfItem);
}

