// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileShooter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Turret.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API ATurret : public AActor, public IProjectileShooter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void VisionSweep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TurretInfo)
	int32 TraceCount; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TurretInfo)
	float ConeAngle; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TurretInfo)
	float ConeLength; 

	FVector ConeOrigin; 
	FVector ConeDirection; 
	
	bool Reloaded;

	float TraceGap;

	bool IsEnemy(AActor* Actor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	USceneComponent* BulletSpawn;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyFiredShot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RayOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RayLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackForce;


	FTimerHandle reloadTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Swivle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = TurretInfo)
	TSubclassOf<class AProjectile> ProjectileType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = TurretInfo)
	int PoolCount;

	TQueue<class AProjectile*> objectPool;
	void CreateObjects();
	void SpawnProjectile();
	void Reload();

	virtual void ShootProjectile() override;

	virtual FVector GetOwnerActorLocation() override;

	virtual USceneComponent* GetBulletSpawn() override;

	virtual void DetermineShotWithRay() override;


};
