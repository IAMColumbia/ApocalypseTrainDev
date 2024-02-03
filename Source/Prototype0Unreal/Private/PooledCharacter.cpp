// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledCharacter.h"

// Sets default values
APooledCharacter::APooledCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APooledCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APooledCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APooledCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APooledCharacter::SpawnPooledCharacter(FVector location, FRotator rotation, bool, FVector target)
{
	CharacterEnabled = true;
	SetActorLocation(location);
	SetActorRotation(rotation);
	//SetActorHiddenInGame(false);
}

void APooledCharacter::DespawnPooledCharacter()
{
	CharacterEnabled = false;
	SetActorLocation(FVector(-4000, 0, 0));
	//SetActorHiddenInGame(true);
}

bool APooledCharacter::isSpawned()
{
	return CharacterEnabled;
}

