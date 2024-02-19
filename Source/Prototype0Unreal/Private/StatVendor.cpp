// Fill out your copyright notice in the Description page of Project Settings.


#include "StatVendor.h"
#include "MyCharacter.h"

FText AStatVendor::getUpgradeText(float value, EUpgradeType type)
{
	return FText::Format(INVTEXT("+{0} {1}"), value, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)type));
}

void AStatVendor::OnInteract(AMyCharacter* player)
{
	
	if (!Dispensing) {
		if (player->Coins >= CostOfItem) {
			OnPurchase();
			Dispensing = true;
			player->LoseCoins(CostOfItem);
			player->ApplyUpgrade(upgradeType, UpgradeAmount);
			PurchasedUpgrade(getUpgradeText(UpgradeAmount, upgradeType), player->PlayerColor);
		}
	}
}


