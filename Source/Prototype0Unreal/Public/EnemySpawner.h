// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UGameManagerWSS;
class AEnemyCharacter;

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
	int DifficultyIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int EnemiesPerChunk;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int MaxEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float RearSpawnRate;

	int ZombiesAlive();

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

	FTimerHandle rearSpawner;

	TArray<AEnemyCharacter*> enemyPool;

public:	
	
	int currentEnemiesPerEncounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int enemiesPerEncounter = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	int enemiesPerEncounterIncrease = 2;

	bool EncounterSpawningComplete;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartRearSpawner();
	void StopRearSpawner();

	void SpawnEnemies();

	void SpawnEnemyBehindTrain();

	void PrintStuff();
	
	FTimerHandle spawnTimerHandle;

	void IncreaseEnemyDifficulty();

	AEnemyCharacter* SpawnPooledEnemy(FVector spawnLocation, FRotator rotation, bool SetTarget, FVector target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyType")
	TSubclassOf<AEnemyCharacter> enemyActorClass;
};
