// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameManagerWSS.h"
#include "Spawners/ChunkSpawner.h"
#include "Train.h"
#include "Spawners/EnemySpawner.h"
#include <Kismet/GameplayStatics.h>
#include "Subsystems/MenuManager.h"
#include "Subsystems/MusicManager.h"

float UGameManagerWSS::GetPriceInflation()
{
	return enemySpawner->PriceInflation;
}

void UGameManagerWSS::EnemyKilled()
{
	if (EncounterActive()) {
		enemySpawner->enemiesKilledThisEncounter++;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("%d killed out of %d"), enemySpawner->enemiesKilledThisEncounter, enemySpawner->enemiesPerEncounter));
		enemySpawner->DisplayKillCounter(enemySpawner->enemiesPerEncounter - enemySpawner->enemiesKilledThisEncounter);
	}
}

void UGameManagerWSS::OnTutorialExit()
{
	musicManager->PlayMainNotify();
}

void UGameManagerWSS::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("GameManager Initialized"));
}


bool UGameManagerWSS::EncounterActive()
{
	if (CurrentGameState == EGameState::encounter) {
		return true;
	}
	return false;
}

void UGameManagerWSS::PrintTrainLocation(FVector location) {
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, FString::Printf(TEXT("Train Location %f"), location.Y));
}

float UGameManagerWSS::GetNextTargetLocation() {
	return chunkSpawner->GetNextTargetLocation();
}

void UGameManagerWSS::SpawnNewChunk()
{
	chunkSpawner->SpawnNextChunk();
	TotalMeters = train->TotalMeters;
	if (train->CanMove && chunkSpawner->TotalChunksSpawned >= 0) {
		if (enemySpawner->DifficultyIncreaseFrequency != 0) {
			if (TotalMeters % enemySpawner->DifficultyIncreaseFrequency == 0 && TotalMeters > 2) {
				enemySpawner->EnemiesPerChunk++;
			}
		}
		enemySpawner->SpawnEnemies();
	}
	train->targetYPos = GetNextTargetLocation();
	train->IncrementTotalMeters();
	CurrentYPos = train->targetYPos;
	
}


FVector UGameManagerWSS::GetRandomLocationInChunk(float zoffset)
{
	//CHANGE this method to fix spawning issues
	float edgeNegation = 100;
	//maybe just add the targetYpos to the y value on the vector to keep things in range. Spawnign on the x and z should be fine if just getting values from the mesh
	if (chunkSpawner == NULL) {
		return FVector(0, 0, 0);
	}
	FVector chunkScale = chunkSpawner->chunkScale;
	float yOffset = chunkSpawner->nextTargetLocation;
	float zOffset = zoffset;
	FVector location = FVector(FMath::RandRange(-chunkScale.X + edgeNegation, chunkScale.X-edgeNegation), FMath::RandRange(-chunkScale.Y + yOffset + edgeNegation, chunkScale.Y + yOffset - edgeNegation), zOffset);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("x: %f y: %f z: %f"), location.X, location.Y, location.Z));

	return location;
}

FVector UGameManagerWSS::GetRandomEncounterSpawnPos()
{
	return train->GetRandomEncounterSpawnPos();
}

FVector UGameManagerWSS::GetTrainLocation()
{

	return train->GetActorLocation();
}

bool UGameManagerWSS::IsOutOfBounds(FVector actorLocation)
{
	if (actorLocation.Y < train->GetBackBound()|| actorLocation.Y > train->GetFrontBound()) {
		return true;
	}
	if (actorLocation.Z < -400) {
		return true;
	}
	return false;
}

bool UGameManagerWSS::IsOutOfBackBounds(FVector actorLocation)
{
	if (train == NULL) {
		return false;
	}
	if (actorLocation.Y < train->GetBackBound()) {
		return true;
	}
	if (actorLocation.Z < -100) {
		return true;
	}
	return false;
}

void UGameManagerWSS::OnTrainAccelerating()
{
	enemySpawner->StopEncounterSpawner();
	
}

void UGameManagerWSS::OnTrainStopped()
{
	if (chunkSpawner->TotalChunksSpawned > 1) {
		//enemySpawner->StartRearSpawner();
	}
}

bool UGameManagerWSS::TrainFuelFull()
{
	if (train != NULL) {
		if (train->Fuel >= train->MaxFuel) {
			return true;
		}
	}
	return false;
}

bool UGameManagerWSS::TrainFuelCritical()
{
	return train->FuelIsCritical();
}

int UGameManagerWSS::TotalChunksSpawned()
{
	if (chunkSpawner == NULL) {
		return 0;
	}
	return chunkSpawner->TotalChunksSpawned;
}

void UGameManagerWSS::GameOver(int loseType)
{
	if (!gameEnded) {
		musicManager->PlayGameOverNotify();
		gameEnded = true;
		menuManager->GameOver(loseType);
	}
}

void UGameManagerWSS::RestartGame()
{
	UGameplayStatics::OpenLevel((UObject*)GetWorld(), FName(TEXT("TrainTest")));
}

void UGameManagerWSS::PlayerJoined()
{
	if (train != NULL) {
		train->PlayerJoined();
	}
}

void UGameManagerWSS::ApproachingStation()
{
	if (train != NULL && enemySpawner != NULL) {
		
		CurrentGameState = EGameState::encounter;
		train->StartHordeEncounter();
		enemySpawner->StartEncounterSpawner();
		//enemySpawner->PriceInflation++;
		musicManager->PlayEncounterNotify();
	}
}

void UGameManagerWSS::EnterStation()
{
	if (train != NULL) {
		IsInShopSequence = true;
		CurrentGameState = EGameState::traveling;
		HordesDefeated++;
		train->StartTrain();
		train->Stopped = false;
		enemySpawner->IncreaseEnemyDifficulty();
		enemySpawner->StopAllEncounterSpawning();
		musicManager->PlayPickupNotify();
		enemySpawner->PriceInflation *= 2;
		chunkSpawner->NotifyHordeDefeated(HordesDefeated);
	}
}

void UGameManagerWSS::ExitStation()
{
	if (train != NULL) {
		CurrentGameState = EGameState::traveling;
		train->Stopped = false;
		train->StartTrain();
		IsInShopSequence = false;
		musicManager->PlayMainNotify();
		train->IncreaseDifficulty();
	}
}

void UGameManagerWSS::EnterShop()
{
	CurrentGameState = EGameState::shopping;
	train->Stopped = true;
	train->StopTrain();
}

bool UGameManagerWSS::TrainIsStopped()
{
	if (train != NULL) {
		return train->IsTrainStopped();
	}
	return false;
}


