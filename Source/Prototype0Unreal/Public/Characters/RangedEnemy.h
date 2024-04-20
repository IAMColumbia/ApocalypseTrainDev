// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedEnemy.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API ARangedEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector getTargetLocation();

	void FireShot();

	float CalculateLaunchVelocity(float gravity, float distance, float angle);

	float CalculateLaunchAngle(float gravity, float distance, float initialVelocity);

public:	

	FVector GetLaunchVector(FVector StartPosition, FVector TargetPosition);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
