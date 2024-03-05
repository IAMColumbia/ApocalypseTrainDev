// Fill out your copyright notice in the Description page of Project Settings.


#include "StatVendor.h"
#include "MyCharacter.h"
#include "GameManagerWSS.h"

FText AStatVendor::getUpgradeText(float value, EUpgradeType type)
{
	return FText::Format(INVTEXT("+{0} {1}"), value, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)type));
}

FText AStatVendor::getInfoText()
{
	return FText::Format(INVTEXT("{0} - {1}"), CostOfItem, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)upgradeType));
}

void AStatVendor::BeginPlay()
{
	Super::BeginPlay();
	CostOfItem = CostOfItem + GetWorld()->GetSubsystem<UGameManagerWSS>()->GetPriceInflation();
}

void AStatVendor::OnInteract(AMyCharacter* player)
{
	if (!Dispensing) {
		if (player->Coins >= CostOfItem) {
			Dispensing = true;
			OnPurchase();
			player->LoseCoins(CostOfItem);
			//GEngine->AddOnScreenDebugMessage(-1, 3, player->GetPlayerColor(), FString::Printf(TEXT("UpgradeAmount %f"), UpgradeAmount));
			player->ApplyUpgrade(upgradeType, UpgradeAmount);
			//GEngine->AddOnScreenDebugMessage(-1, 3, player->GetPlayerColor(), FString::Printf(TEXT("Index: %d"), player->PlayerIndex));
			PurchasedUpgrade(getUpgradeText(UpgradeAmount, upgradeType), player->PlayerColor);
		}
	}
}


