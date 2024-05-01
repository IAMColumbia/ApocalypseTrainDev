// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "MeleeWeapon.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMyCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackForce;

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UBoxComponent* box;

	bool CanDamage;

	UFUNCTION(BlueprintCallable)
	void CallAttackEvent();

	UFUNCTION(BlueprintCallable)
	void CallIdleEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void Attacking();

	UFUNCTION(BlueprintImplementableEvent)
	void Idle();

	UFUNCTION(BlueprintImplementableEvent)
	void HitObstacle();

	UFUNCTION(BlueprintImplementableEvent)
	void HitEnemy();
};
