// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileShooter.h"
#include "Weapon.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AWeapon : public AActor, public IProjectileShooter
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWeapon();

    class AMyCharacter* OwnerCharacter;

    virtual FVector GetOwnerActorLocation() override;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int OwnerPlayerIndex();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
    int WeaponNumber;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsOwnerDead();



    UPROPERTY(BlueprintReadOnly)
    FVector hitLocation;
    UPROPERTY(BlueprintReadOnly)
    FVector hitNormal;
    UFUNCTION(BlueprintImplementableEvent)
    void SpawnBulletVFX(FVector location, FVector normal, int type);

    void RaiseWeapon();
    void LowerWeapon();

    void KilledEnemy();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FVector GetBeamEnd();

    USceneComponent* BulletSpawn;
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool Attacking;
    bool prevAttackState;
    virtual void Attack();
    virtual void EndAttack();
    void UpdateAttackingState();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RayOffset;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RayLength;
    void GetRayVector(FVector*& rayStart, FVector*& rayEnd);

    virtual void DetermineShotWithRay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float KnockbackForce;
    UFUNCTION(BlueprintImplementableEvent)
    void NotifyFiredShot();

    bool Equipped;
    void WeaponEquipped();
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Audio")
    USoundBase* equipSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Audio")
    USoundAttenuation* attenuation;

    bool Reloaded;
    float currentReloadTime;
    void Reload();
    void UpdateReloadTime();

    USceneComponent* laser;
    void HideLaser();
    void ShowLaser();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<class AProjectile> ProjectileType;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    int PoolCount;

    TQueue<class AProjectile*> objectPool;
    void CreateObjects();
    virtual void SpawnProjectile();

    bool upright;
    FQuat OriginalRotation;
    bool IsWeaponClipping();

    virtual USceneComponent* GetBulletSpawn() override;

    UFUNCTION(BlueprintImplementableEvent)
    void NotifyStoppedShooting();

    UFUNCTION(BlueprintImplementableEvent)
    void OwnerDied();

    UFUNCTION(BlueprintCallable)
    virtual void ApplyPlayerKnockback();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float KnockbackPlayerForce;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    FTimerHandle shootTimerHandle;
    FTimerHandle reloadTimerHandle;
    virtual void ShootProjectile() override;

    USceneComponent* weaponRotator;

    void SpawnHitVFXType(FHitResult hit);

};