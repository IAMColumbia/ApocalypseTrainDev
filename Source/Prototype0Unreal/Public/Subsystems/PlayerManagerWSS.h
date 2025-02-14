// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerManagerWSS.generated.h"

/**
 * 
 */
class AMyCharacter;

UCLASS()
class PROTOTYPE0UNREAL_API UPlayerManagerWSS : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	TArray<AMyCharacter*> Players;

	void RegisterPlayer(AMyCharacter* player);

	UFUNCTION(BlueprintCallable)
	bool IsOverlappingPlayer(class UBoxComponent* box);

	UFUNCTION(BlueprintCallable)
	bool IsOverlappingPlayerWithFuel(class UBoxComponent* box);

	UFUNCTION(BlueprintCallable)
	int NumActivePlayers();

	void CheckGameOver();

	UFUNCTION(BlueprintCallable)
	TArray<class TSubclassOf<AWeapon>> GetPlayerWeaponTypes();

	UFUNCTION(BlueprintCallable)
	TArray<int> GetPlayerKills();

	UFUNCTION(BlueprintCallable)
	TArray<int> GetPlayerDeaths();

	float GetAverageLocation();

	void ShareCoins(int coins);

	UFUNCTION(BlueprintCallable)
	FVector GetRandomPlayerLocation();

	UFUNCTION(BlueprintCallable)
	bool AnyPlayerIsCarrying();

	UFUNCTION(BlueprintCallable)
	TArray<struct FPlayerInfo> GetPlayerInfos();

	UFUNCTION(BlueprintCallable)
	AMyCharacter* GetRandomPlayer();

	UFUNCTION(BlueprintCallable)
	bool PlayerHasSpawned();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SortPlayers();
	bool AllPlayersDead();
	
	
};
