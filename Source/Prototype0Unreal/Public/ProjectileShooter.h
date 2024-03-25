// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileShooter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProjectileShooter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROTOTYPE0UNREAL_API IProjectileShooter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void DetermineShotWithRay() = 0;

	virtual void ShootProjectile() = 0;

	virtual FVector GetOwnerActorLocation() = 0;

	virtual USceneComponent* GetBulletSpawn() = 0;
};
