// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/PlayerManagerWSS.h"
#include "Characters/MyCharacter.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Weapons/Weapon.h>
#include <Subsystems/GameManagerWSS.h>
#include <Components/SphereComponent.h>

void UPlayerManagerWSS::RegisterPlayer(AMyCharacter* player)
{
	Players.Add(player);
	SortPlayers();
	GEngine->AddOnScreenDebugMessage(-1, 1, player->GetPlayerColor(), FString::Printf(TEXT("Player %d was registered with manager"), player->PlayerIndex));
	GetWorld()->GetSubsystem<UGameManagerWSS>()->PlayerJoined();
	//Players[player->PlayerIndex] = player;
}

bool UPlayerManagerWSS::IsOverlappingPlayer(UBoxComponent* box)
{
	for (int i = 0; i < Players.Num(); i++) {
		if (UKismetMathLibrary::IsPointInBox(Players[i]->GetActorLocation(), box->GetComponentLocation(), box->GetScaledBoxExtent()))
		{
			return true;
		}
	}
	return false;
}

bool UPlayerManagerWSS::IsOverlappingPlayerWithFuel(UBoxComponent* box)
{
	for (int i = 0; i < Players.Num(); i++) {
		if (UKismetMathLibrary::IsPointInBox(Players[i]->GetActorLocation(), box->GetComponentLocation(), box->GetScaledBoxExtent()))
		{
			if (Players[i]->HasFuel()) {
				return true;
			}
		}
	}
	return false;
}

int UPlayerManagerWSS::NumActivePlayers()
{
	return Players.Num();
}

void UPlayerManagerWSS::CheckGameOver()
{
	//NOT HAVING PLAYERS DYING END THE GAME
	if (AllPlayersDead() && Players.Num() > 1) {
		
		GetWorld()->GetSubsystem<UGameManagerWSS>()->GameOver(1);
	}
}

TArray< TSubclassOf<AWeapon>> UPlayerManagerWSS::GetPlayerWeaponTypes()
{
	TArray<TSubclassOf<AWeapon>> weaponarray;
	int activePlayers = NumActivePlayers();
	for (int i = 0; i < activePlayers; i++) {
		weaponarray.Add(Players[i]->GetCurrentWeapon());
	}
	return weaponarray;
}

TArray<int> UPlayerManagerWSS::GetPlayerKills()
{
	TArray<int> kills;
	for (int i = 0; i < Players.Num(); i++) {
		kills.Add(Players[i]->TotalKills);
	}
	return kills;
}

TArray<int> UPlayerManagerWSS::GetPlayerDeaths()
{
	TArray<int> deaths;
	for (int i = 0; i < Players.Num(); i++) {
		deaths.Add(Players[i]->TotalDeaths);
	}
	return deaths;
}

float UPlayerManagerWSS::GetAverageLocation()
{
	TArray<float> positions;
	for (int i = 0; i < Players.Num(); i++) {
		if (!Players[i]->IsPlayerDead) {
			positions.Add(Players[i]->GetActorLocation().Y);
		}
	}
	float min = FMath::Min(positions);
	float max = FMath::Max(positions);
	float average = (min + max) / 2;
	return average;
}

void UPlayerManagerWSS::ShareCoins(int coins)
{
	for(AMyCharacter * c : Players) {
		c->AddCoins(coins);
	}
}

FVector UPlayerManagerWSS::GetRandomPlayerLocation()
{
	if (Players.Num() == 0) {
		return FVector(0, 0, 0);
	}
	if (Players.Num() == 1) {
		return Players[0]->GetActorLocation();
	}
	int player = FMath::RandRange(0, NumActivePlayers()-1);
	return Players[player]->GetActorLocation();
}

bool UPlayerManagerWSS::AnyPlayerIsCarrying()
{
	for (AMyCharacter* c : Players) {
		if (c->Carrying) {
			return true;
		}
	}
	return false;
}

TArray<struct FPlayerInfo> UPlayerManagerWSS::GetPlayerInfos()
{
	TArray<struct FPlayerInfo> infos = TArray<struct FPlayerInfo>();
	for(AMyCharacter* c : Players)
	{
		infos.Add(c->GetPlayerInfo());
	}
	return infos;
}

AMyCharacter* UPlayerManagerWSS::GetRandomPlayer()
{
	int player = FMath::RandRange(0, NumActivePlayers() - 1);
	return Players[player];
}

bool UPlayerManagerWSS::PlayerHasSpawned()
{
	if (Players.Num() <= 0) {
		return false;
	}
	return true;
}

bool UPlayerManagerWSS::AllPlayersDead()
{
	int deadPlayers = 0;
	int activePlayers = NumActivePlayers();
	if (activePlayers > 0) {
		for (int i = 0; i < activePlayers; i++) {
			if (Players[i]->IsPlayerDead == true) {
				deadPlayers++;
			}
		}
	}
	if (deadPlayers >= activePlayers) {
		return true;
	}
	return false;
}


void UPlayerManagerWSS::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerManager Initialized"));
}

void UPlayerManagerWSS::SortPlayers()
{
	if (Players.Num() <= 1) {
		return;
	}
	for (int i = 1; i < Players.Num(); i++) {
		if (Players[i]->PlayerIndex > Players[i - 1]->PlayerIndex) {
			AMyCharacter* temp = Players[i - 1];
			Players[i - 1] = Players[i];
			Players[i] = temp;
		}
	}
}








