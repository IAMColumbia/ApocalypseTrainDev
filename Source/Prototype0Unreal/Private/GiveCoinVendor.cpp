// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/GiveCoinVendor.h"
#include "Characters/MyCharacter.h"

void AGiveCoinVendor::CheckForInteractPressed()
{
	if (wasInteracted) {
		return;
	}
	for(AMyCharacter* player : overlappingPlayers) {
		if (player->Interacted) {
			OnInteract(player);
			//Figure out why player index not working
			//GEngine.AddOnScreenMessage()
		}
	}
}

void AGiveCoinVendor::OnInteract(AMyCharacter* player)
{
	if (!Dispensing) {
		if (player->carriedObject != NULL) {
			if (player->carriedObject->ActorHasTag("Fuel")) {
				Dispensing = true;
				player->carriedObject->Destroy();
				OnPurchase();
			}
		}
	}
}
