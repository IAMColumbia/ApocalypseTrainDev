// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledCharacter.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class EEnemyState : uint8 {
	Alive UMETA(DisplayName = "Alive"), Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class PROTOTYPE0UNREAL_API AEnemyCharacter : public APooledCharacter
{
	GENERATED_BODY()

public:

	bool InViewOfPlayer;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyInViewOfPlayer();

	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDamageEnemy(float Damage, bool showDamage);

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyKilled(bool dropLoot);
public:	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void NotifyCanDamagePlayer(bool CanDamage);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float currentHealth;
	//how far away from the player before the enemy starts playing the attack animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isDamaged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float enemyHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knockback")
	bool WasKnocked;
	FVector KnockedDirection;
	float KnockSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Knockback")
	float KnockbackDeceleration;

	void ApplyKnockback();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool TakeDamage(float distance, float damage, FVector sourcePos, float launchForce, bool dropLoot);

	UFUNCTION(BlueprintCallable)
	bool KillEnemy(bool dropLoot);

	UFUNCTION(BlueprintCallable)
	void Knockback(FVector direction, float force);
	
	UFUNCTION(BlueprintCallable)
	bool IsInAttackRange(AActor* targetToAttack);

	UFUNCTION(BlueprintImplementableEvent)
	void upgradeEnemy(float HealthIncrease, float DmgIncrease, float SpeedIncrease);

	virtual void SpawnPooledCharacter(FVector location, FRotator rotation, bool setTarget, FVector target) override;

	virtual void DespawnPooledCharacter() override;

	UFUNCTION(BlueprintImplementableEvent)
	void EnemySpawned(bool setTarget, FVector target);

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyDespawned();

};
