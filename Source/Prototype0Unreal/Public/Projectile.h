// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AProjectile : public APooledActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FVector storedHitNormal;
	UPROPERTY(BlueprintReadOnly)
	FVector storedHitLocation;

	virtual void Tick(float DeltaTime) override;

	virtual void Spawn() override;

	virtual void SpawnWithRotation(FRotator rot);
	UFUNCTION(BlueprintCallable)
	virtual void Despawn() override;
	virtual void InitializeProjectile(class AWeapon* owner);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Lifetime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpeed;
	
	bool Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Despawning;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetOwnerCharacterLocation();

protected:
	class AWeapon* Owner;
	USceneComponent* spawnPoint;
	FTimerHandle lifetimeHandle;

	void MoveProjectile();
};
