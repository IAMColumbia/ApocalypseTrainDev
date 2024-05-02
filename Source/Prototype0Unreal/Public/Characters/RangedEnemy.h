// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Obstacle.h"
#include "GameFramework/Actor.h"
#include "RangedEnemy.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API ARangedEnemy : public AObstacle
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector getTargetLocation();


	float CalculateLaunchVelocity(float gravity, float distance, float angle);

	float CalculateLaunchAngle(float gravity, float distance, float initialVelocity);

	class UPlayerManagerWSS* playerManager;

	FTimerHandle shootTimer;
	class AMyCharacter* currentTarget;

public:	
	UFUNCTION(BlueprintCallable)
	void FireShot();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAttacking;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLaunchVector(FVector StartPosition, FVector TargetPosition);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> enemyProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float randomFireRateDeviation;

	virtual void DamageObstacle(float damage) override;

	//virtual void SpawnPooledCharacter(FVector location, FRotator rotation, bool setTarget, FVector target) override;

	//virtual void DespawnPooledCharacter() override;
};
