// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/StationChunk.h"
#include "Subsystems/GameManagerWSS.h"
#include "Spawners/EnemySpawner.h"


bool AStationChunk::isEnterable()
{
    AEnemySpawner* spawner = GetWorld()->GetSubsystem<UGameManagerWSS>()->enemySpawner;
    if (spawner->ZombiesAlive() > 0) {
        return false;
    }
    return true;
}

bool AStationChunk::isExitable()
{
    return false;
}
