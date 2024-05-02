// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameManagerWSS.generated.h"

/**
 * 
 */
UENUM()
enum class EGameState : uint8 {
	traveling UMETA(DisplayName = "traveling"), encounter UMETA(DIsplayName = "encounter"), shopping UMETA(DIsplayName = "shopping")
};

UCLASS()
class PROTOTYPE0UNREAL_API UGameManagerWSS : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsInShopSequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int HordesDefeated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState CurrentGameState;

	bool EncounterActive();

	class AChunkSpawner* chunkSpawner;
	class AEnemySpawner* enemySpawner;
	class ATrain* train;
	class AMenuManager* menuManager;
	class AMusicManager* musicManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentYPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalMeters;

	void PrintTrainLocation(FVector location);
	float GetNextTargetLocation();
	void SpawnNewChunk();

	UFUNCTION(BlueprintCallable)
	FVector GetRandomLocationInChunk(float zoffset);

	FVector GetRandomEncounterSpawnPos();

	UFUNCTION(BlueprintCallable)
	FVector GetTrainLocation();

	bool IsOutOfBounds(FVector actorLocation);

	UFUNCTION(BlueprintCallable)
	bool IsOutOfBackBounds(FVector actorLocation);

	void OnTrainAccelerating();
	void OnTrainStopped();

	UFUNCTION(BlueprintCallable)
	bool TrainFuelFull();

	UFUNCTION(BlueprintCallable)
	bool TrainFuelCritical();

	int TotalChunksSpawned();

	void GameOver(int loseType);
	bool gameEnded;

	void RestartGame();

	void PlayerJoined();

	UFUNCTION(BlueprintCallable)
	void ApproachingStation();
	UFUNCTION(BlueprintCallable)
	void EnterStation();
	UFUNCTION(BlueprintCallable)
	void ExitStation();
	UFUNCTION(BlueprintCallable)
	void EnterShop();
	UFUNCTION(BlueprintCallable)
	bool TrainIsStopped();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool waitingForShopEnterDoors;

	float GetPriceInflation();

	void EnemyKilled();

	void OnTutorialExit();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
