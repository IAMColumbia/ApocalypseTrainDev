// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MaxObstacleHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float currentHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NotDamageable;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void DamageObstacle(float damage);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDamageObstacle(float damage);

	UFUNCTION(BlueprintImplementableEvent)
	void ObstacleDestroyed();
};
