// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UPlayerWeapon;
class AWeapon;
class AMeleeWeapon;

UENUM(BlueprintType)
enum class EUpgradeType : uint8{damage UMETA(DisplayName="Damage"), health UMETA(DisplayName = "Health"), speed UMETA(DisplayName = "Handling")
};

USTRUCT(BlueprintType)
struct FPlayerInfo {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor PlayerColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Kills;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FuelDeposited;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerNumber;
};

//damage types 
//4 - train
//5 - out of bounds
//0 - zombie
//1 - explosion
//2 - river
//3 - slimed

UCLASS()
class PROTOTYPE0UNREAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AMyCharacter();

	bool IsFacingWall();

	float deadZone = 0.45f;
	//how much is added to players base damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float DamageBuff;

	TSubclassOf<AWeapon> PickupWeapon(TSubclassOf<AWeapon> weaponToPickup);

	TSubclassOf<AWeapon> GetCurrentWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalKills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalDeaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalFuelDeposited;

	void IncreaseTotalFuel();

	UFUNCTION(BlueprintCallable)
	FPlayerInfo GetPlayerInfo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Coins;

	UFUNCTION(BlueprintCallable)
	void LoseCoins(int coinsToLose);

	UFUNCTION(BlueprintCallable)
	void AddCoins(int coinsToAdd);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCoinUI();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool Bashing;

	UFUNCTION(BlueprintCallable)
	void BashFinished();

	void OnEnemyKilled();

protected:

	void CheckForEnemyShowHealth();

	void AttachWeapon();
	void HolsterWeapon();

	void HolsterMeleeWeapon();
	void AttachMeleeWeapon();

	class UGameManagerWSS* gameManager;
	class ATrain* trainPtr;

	FVector aimRot;
	void setXRot(float AxisValue);
	void setYRot(float AxisValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AMeleeWeapon> MeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMeleeWeapon* CurrentMeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat")
	bool IsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate;

	//how much damage players do to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	//the rate at which players passivly increase health if below max
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RegenRate;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float SpeedBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float MaxSpeedBuff;

	float baseHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool CanAddFuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float KnockBackDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	bool isDashing;

	bool canDash;

	float DashSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDecelleration;
	void ApplyDash();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyOnDash(FVector direction);

	UFUNCTION(BLueprintImplementableEvent)
	void OnDashFinish();

	//UFUNCTION();
	void ResetDash();
	void FinishDash();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector DashDirection;

	FTimerHandle dashCooldownTimerHandle;
	FTimerHandle dashTimerHandle;

	FTimerHandle shootTimerHandle;
	FTimerHandle respawnTimerHandle;

	bool Invincible;
	void SetInvincibility();

	//How long it takes the player to respawn when they die
	UPROPERTY(EditAnywhere, Category = "Respawning")
	int TotalRespawnTime;
	int currentRespawnTime;

	UFUNCTION(BlueprintCallable, Category = "Respawning")
	void UpdateRespawnTimer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void setRotation();

	void ShootPressed();
	void ShootReleased();

	void DashPressed();

	void BashPressed();

	

	void InteractPressed();
	void InteractReleased();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyWeaponEquipped(int weaponNum);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStartedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStoppedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerDied(int deathReason);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerRespawn();

	UFUNCTION(BlueprintImplementableEvent)
	void SetMovementSpeed(float speed);

public:	
#pragma region UpgradeAmounts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpgradeAmounts")
	float DamageUpgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpgradeAmounts")
	float SpeedUpgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpgradeAmounts")
	float HandlingUpgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpgradeAmounts")
	float MaxHealthUpgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpgradeAmounts")
	float RegenRateUpgrade;

#pragma endregion

#pragma region StatLevels
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	int DamageLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	int HealthLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	int SpeedLevel;
#pragma endregion


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float HandelingBuff;


	UFUNCTION(BlueprintImplementableEvent)
	void NotifyShot();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool isWeaponUpright();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Interacted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Carrying;

	bool justDropped;

	class AInteractableActor* carriedObject;

	USceneComponent* CarrySlot;
	USceneComponent* HolsterSlot;

	void PickupItem(class AInteractableActor* itemToCarry);

	void CheckDropItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Mesh")
	USkeletalMeshComponent* characterMesh;

	UFUNCTION(BlueprintCallable, Category = "Respawning")
	void ResetPlayer();

	UFUNCTION(BlueprintCallable, Category = "Respawning")
	FVector GetRespawnLocation();

	//Player health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsPlayerDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float currentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Initialize")
	int PlayerIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Initialize")
	FVector PlayerColor;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float damageToTake, float type);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTakeDamage();

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void OnPlayerSpawn();

	FColor GetPlayerColor();

	FVector SetPlayerColorVector(int index);

	bool HasFuel();

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyUpgrade(EUpgradeType upgrade, float amount);

	UFUNCTION(BlueprintCallable)
	void Heal();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyReloadPercent(float currentTime, float MaxTime);

	bool overlappingInteractable;

	UFUNCTION(BlueprintCallable)
	void DespawnPlayer(float deathtype);
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> ActorToSpawn;


	void RegenerateHealth();

};
