// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UGameManagerWSS;
class AEnemyCharacter;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyCharacter> enemyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxPooledEnemies;
};

UCLASS()
class PROTOTYPE0UNREAL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Maximum Enemies")
	int MaximumPooledEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Initial Values")
	float EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Initial Values")
	float EnemySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Initial Values")
	float EnemyDamage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy Initial Values")
	FVector initialEnemyPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty Increase")
	float SpawnRateIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty Increase")
	float EnemyHealthIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty Increase")
	float EnemySpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty Increase")
	float EnemyDamageIncrease;

	//a wave divisible by this number will increase the enemy difficulty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Difficulty Increase")
	int DifficultyIncreaseFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int EnemiesPerChunk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int MaxEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float RearSpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float RearAftermathSpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int enemiesPerEncounter = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int enemiesPerEncounterIncrease = 2;

	bool EncounterSpawningComplete;

	int currentEnemiesPerEncounter;
	int enemiesKilledThisEncounter;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	float PriceInflation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifySpawnEnemy(FVector spawnLocation, FRotator rotation, bool SetTarget, FVector target);

	//TQueue<AEnemyCharacter*> objectPool;
	//void PopulatePool();

	TArray<AEnemyCharacter*> encounterEnemies;
	bool encounterEnemiesKilled();

	void KillRemainingEncounterEnemies();

	FTimerHandle encounterSpawnTimerHandle;

	TArray<AEnemyCharacter*> createEnemyPool(TSubclassOf<AEnemyCharacter> enemyClass, int MaxPool);
	
	TArray<TArray<AEnemyCharacter*>> enemyPoolArray;

public:	



	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartEncounterSpawner();
	void StopEncounterSpawner();
	
	int ZombiesAlive();

	void SpawnEnemies();

	void SpawnEncounterEnemy();
	
	void IncreaseEnemyDifficulty();

	void StopAllEncounterSpawning();

	AEnemyCharacter* SpawnPooledEnemy(FVector spawnLocation, FRotator rotation, bool SetTarget, FVector target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyType")
	TArray<FEnemySpawnInfo> enemyTypes;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayKillCounter(int enemiesNeeded);

	UFUNCTION(BlueprintImplementableEvent)
	void HideKillCounter();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayHordeWarning();

};
