// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "GameManagerWSS.h"
#include "EnemyCharacter.h"
#include <Kismet/KismetSystemLibrary.h>
#include "GameManagerWSS.h"
// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

int AEnemySpawner::ZombiesAlive()
{
	int alive = 0;
	for (AEnemyCharacter* e : enemyPool) {
		if (e->isSpawned()) {
			alive++;
		}
	}
	return alive;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->enemySpawner = this;
	for (int i = 0; i < MaximumPooledEnemies; i++) {
		AEnemyCharacter* enemy = GetWorld()->SpawnActor<AEnemyCharacter>(enemyActorClass, FVector(-4000, 0, 0 ),FRotator(0,0,0), FActorSpawnParameters());
		if (enemy != NULL) {
			enemyPool.Add(enemy);
			enemy->DespawnPooledCharacter();
		}
	}
	EncounterSpawningComplete = true;
}

bool AEnemySpawner::encounterEnemiesKilled()
{
	if (encounterEnemies.Num() > 0) {
		for (AEnemyCharacter* e : encounterEnemies) {
			if (e->isSpawned()) {
				return false;
			}
		}
	}
	return true;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EncounterSpawningComplete && GetWorld()->GetSubsystem<UGameManagerWSS>()->EncounterActive() && encounterEnemiesKilled()) {
		//GetWorld()->GetSubsystem<UGameManagerWSS>()->EnterStation();
		GetWorld()->GetSubsystem<UGameManagerWSS>()->waitingForShopEnterDoors = true;
		encounterEnemies.Empty();
	}
}

void AEnemySpawner::StartRearSpawner()
{
	if (EncounterSpawningComplete == true) {
		EncounterSpawningComplete = false;
		currentEnemiesPerEncounter = enemiesPerEncounter;
		GetWorld()->GetTimerManager().SetTimer(rearSpawner, this, &AEnemySpawner::SpawnEnemyBehindTrain, RearSpawnRate, true);
		int chunks = GetWorld()->GetSubsystem<UGameManagerWSS>()->TotalChunksSpawned();
		RearSpawnRate -= (chunks * SpawnRateIncrease);
	}
}

void AEnemySpawner::StopRearSpawner()
{
	GetWorld()->GetTimerManager().ClearTimer(rearSpawner);
	EncounterSpawningComplete = true;
}

void AEnemySpawner::SpawnEnemies()
{
	for (int i = 0; i < EnemiesPerChunk; i++) {
		FVector spawnLocation = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomLocationInChunk(100);
		SpawnPooledEnemy(spawnLocation, FRotator(0, 0, (float)FMath::RandRange(0, 180)), false, FVector());
	}
	EnemiesPerChunk++;
}

void AEnemySpawner::SpawnEnemyBehindTrain()
{
	if (ZombiesAlive() == enemyPool.Num() || enemyPool.Num() == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("OutOfEnemiesToSpawnForEncounter"));
		StopRearSpawner();
		return;
	}
	FVector spawnLoc = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomLocationBehindTrain();
	FVector trainLoc = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetTrainLocation();
	AEnemyCharacter* e = SpawnPooledEnemy(spawnLoc + FVector(0,0,400), FRotator().ZeroRotator, true, FVector(400, trainLoc.Y + 500, 20));
	if (e != NULL) {
		encounterEnemies.Add(e);
	}
	currentEnemiesPerEncounter--;
	if (currentEnemiesPerEncounter <= 0) {
		StopRearSpawner();
		enemiesPerEncounter += enemiesPerEncounterIncrease;
	}
}

void AEnemySpawner::PrintStuff()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("I am enemy spawner"));
}

void AEnemySpawner::IncreaseEnemyDifficulty()
{
	EnemySpeed += EnemySpeedIncrease;
	EnemyHealth += EnemyHealthIncrease;
	EnemyDamage += EnemyDamageIncrease;
}

AEnemyCharacter* AEnemySpawner::SpawnPooledEnemy(FVector spawnLocation, FRotator rotation, bool SetTarget, FVector target)
{
	if (enemyPool.IsEmpty()) {
		return NULL;
	}
	for (AEnemyCharacter* enemy : enemyPool) {
		if (enemy != NULL) {
			if (!enemy->isSpawned()) {
				enemy->upgradeEnemy(EnemyHealth, EnemyDamage, EnemySpeed);
				enemy->SpawnPooledCharacter(spawnLocation, rotation, SetTarget, target);
				return enemy;
			}
		}
	}
	return NULL;
}

