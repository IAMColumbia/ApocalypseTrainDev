// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PooledCharacter.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API APooledCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APooledCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SpawnPooledCharacter(FVector location, FRotator rotation, bool setTarget, FVector target);
	UFUNCTION(BlueprintCallable)
	virtual void DespawnPooledCharacter();

	bool isSpawned();

	bool CharacterEnabled;

};
