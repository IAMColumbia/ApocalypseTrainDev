// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/StatVendor.h"
#include "Characters/MyCharacter.h"
#include "Subsystems/GameManagerWSS.h"

FText AStatVendor::getUpgradeText(float value, EUpgradeType type)
{
	return FText::Format(INVTEXT("Level {0} {1}"), value, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)type));
}

FText AStatVendor::getInfoText()
{
	//return FText::Format(INVTEXT("{0} - {1}"), CostOfItem, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)upgradeType));
	return FText::Format(INVTEXT("{0}"), CostOfItem);
}

void AStatVendor::BeginPlay()
{
	CostOfItem = CostOfItem + GetWorld()->GetSubsystem<UGameManagerWSS>()->GetPriceInflation();
	Super::BeginPlay();
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
			float upgradeLevel = 0;
			switch (upgradeType) {
			case EUpgradeType::speed:
				upgradeLevel = player->SpeedLevel;
				break;
			case EUpgradeType::damage:
				upgradeLevel = player->DamageLevel;
				break;
			case EUpgradeType::health:
				upgradeLevel = player->HealthLevel;
				break;
			}
			upgradeLevel += 1;
			PurchasedUpgrade(getUpgradeText(upgradeLevel, upgradeType), player->PlayerColor);
		}
	}
}


