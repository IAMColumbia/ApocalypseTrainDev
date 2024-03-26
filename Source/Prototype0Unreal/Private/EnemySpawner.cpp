// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawners/EnemySpawner.h"
#include "Subsystems/GameManagerWSS.h"
#include "Characters/EnemyCharacter.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Subsystems/GameManagerWSS.h"




// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

int AEnemySpawner::ZombiesAlive()
{
	int alive = 0;
	for (TArray<AEnemyCharacter*> pool : enemyPoolArray) {
		for (AEnemyCharacter* e : pool) {
			if (e->isSpawned()) {
				alive++;
			}
		}
	}
	return alive;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->enemySpawner = this;
	for (FEnemySpawnInfo spawnInfo : enemyTypes) {
		enemyPoolArray.Add(createEnemyPool(spawnInfo.enemyType, spawnInfo.MaxPooledEnemies));
	}
	EncounterSpawningComplete = true;
}


bool AEnemySpawner::encounterEnemiesKilled()
{
	if (enemiesKilledThisEncounter >= enemiesPerEncounter) {
		return true;
	}
	return false;
}

void AEnemySpawner::KillRemainingEncounterEnemies()
{
	for (AEnemyCharacter* e : encounterEnemies) {
		if (e->isSpawned()) {
			e->TakeDamage(0, 10000000, e->GetActorLocation(), 0, false);
		}
	}
	encounterEnemies.Empty();
}

TArray<AEnemyCharacter*> AEnemySpawner::createEnemyPool(TSubclassOf<AEnemyCharacter> enemyClass, int MaxPool)
{
	TArray<AEnemyCharacter*> pool;
	for (int i = 0; i < MaxPool; i++) {
		AEnemyCharacter* enemy = GetWorld()->SpawnActor<AEnemyCharacter>(enemyClass, initialEnemyPosition, FRotator(0, 0, 0), FActorSpawnParameters());
		if (enemy != NULL) {
			pool.Add(enemy);
			enemy->DespawnPooledCharacter();
		}
	}
	return pool;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EncounterSpawningComplete && GetWorld()->GetSubsystem<UGameManagerWSS>()->EncounterActive() && encounterEnemiesKilled()) {
		GetWorld()->GetSubsystem<UGameManagerWSS>()->waitingForShopEnterDoors = true;
		enemiesPerEncounter += enemiesPerEncounterIncrease;
		KillRemainingEncounterEnemies();
		GetWorld()->GetTimerManager().ClearTimer(encounterSpawnTimerHandle);
		HideKillCounter();
	}
}

void AEnemySpawner::StartEncounterSpawner()
{
	if (EncounterSpawningComplete == true) {
		enemiesKilledThisEncounter = 0;
		EncounterSpawningComplete = false;
		currentEnemiesPerEncounter = enemiesPerEncounter;
		GetWorld()->GetTimerManager().SetTimer(encounterSpawnTimerHandle, this, &AEnemySpawner::SpawnEncounterEnemy, RearSpawnRate, true);
		int chunks = GetWorld()->GetSubsystem<UGameManagerWSS>()->TotalChunksSpawned();
		RearSpawnRate -= (chunks * SpawnRateIncrease);
		if (RearSpawnRate <= 0) {
			RearSpawnRate = 0.01f;
		}
		DisplayKillCounter(enemiesPerEncounter);
		DisplayHordeWarning();
	}
}

void AEnemySpawner::StopEncounterSpawner()
{
	StopAllEncounterSpawning();
	EncounterSpawningComplete = true;
	GetWorld()->GetTimerManager().SetTimer(encounterSpawnTimerHandle, this, &AEnemySpawner::SpawnEncounterEnemy, RearAftermathSpawnRate, true);
}

void AEnemySpawner::SpawnEnemies()
{
	for (int i = 0; i < EnemiesPerChunk; i++) {
		FVector spawnLocation = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomLocationInChunk(100);
		SpawnPooledEnemy(spawnLocation, FRotator(0, 0, (float)FMath::RandRange(0, 180)), false, FVector());
	}
}

void AEnemySpawner::SpawnEncounterEnemy()
{
	FVector spawnLoc = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomEncounterSpawnPos();
	FVector trainLoc = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetTrainLocation();
	AEnemyCharacter* e = SpawnPooledEnemy(spawnLoc + FVector(0,0,400), FRotator().ZeroRotator, true, FVector(400, trainLoc.Y + 500, 20));
	if (e != NULL) {
		encounterEnemies.Add(e);
	}
	if (!EncounterSpawningComplete) {
		currentEnemiesPerEncounter--;
		if (currentEnemiesPerEncounter <= 0) {
			StopEncounterSpawner();
		}
	}
}

void AEnemySpawner::IncreaseEnemyDifficulty()
{
	EnemySpeed += EnemySpeedIncrease;
	EnemyHealth += EnemyHealthIncrease;
	EnemyDamage += EnemyDamageIncrease;
}

AEnemyCharacter* AEnemySpawner::SpawnPooledEnemy(FVector spawnLocation, FRotator rotation, bool SetTarget, FVector target)
{
	int encountersCompleted = GetWorld()->GetSubsystem<UGameManagerWSS>()->HordesDefeated;
	int enemyType = FMath::RandRange(0,encountersCompleted);
	enemyType = FMath::Clamp(enemyType, 0, enemyPoolArray.Num() - 1);
	if (!enemyPoolArray[enemyType].IsEmpty()) {
		for (AEnemyCharacter* enemy : enemyPoolArray[enemyType]) {
			if (enemy != NULL) {
				if (!enemy->isSpawned()) {
					enemy->upgradeEnemy(EnemyHealth, EnemyDamage, EnemySpeed);
					enemy->SpawnPooledCharacter(spawnLocation, rotation, SetTarget, target);
					return enemy;
				}
			}
		}
	}
	return NULL;
}

void AEnemySpawner::StopAllEncounterSpawning()
{
	GetWorld()->GetTimerManager().ClearTimer(encounterSpawnTimerHandle);
}

