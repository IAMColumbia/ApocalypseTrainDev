// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/EnemyCharacter.h"
#include <GameFramework/Actor.h>
#include "Subsystems/GameManagerWSS.h"
#include <Kismet/KismetMathLibrary.h>
#include "Characters/MyCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = MaxHealth;
	NotifyHealthBarWidget();


	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(UCapsuleComponent::StaticClass())) {
			UCapsuleComponent* capsule = Cast<UCapsuleComponent>(component);
			if (capsule && capsule->ComponentHasTag("Capsule"))
			{
				enemyHeight = capsule->GetScaledCapsuleHalfHeight();
			}
		}
	}
}

void AEnemyCharacter::ApplyKnockback()
{
	if (WasKnocked) {
		FHitResult hit;
		FVector start = GetActorLocation();
		FVector end = start + (KnockedDirection * 70);
		if (GetWorld()) {
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
			//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
			if (actorHit && hit.GetActor() && !Cast<AEnemyCharacter>(hit.GetActor())) {

			}
			else {
				FVector loc = GetActorLocation() + (KnockedDirection * KnockSpeed);
				SetActorLocation(loc);
			}
		}
		KnockSpeed -= KnockbackDeceleration;
		if (KnockSpeed <= 0) {
			KnockSpeed = 0;
			WasKnocked = false;
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetSubsystem<UGameManagerWSS>()->IsOutOfBackBounds(GetActorLocation())) {
		DespawnPooledCharacter();
	}
	switch (EnemyState) {
		case EEnemyState::Dead:
			break;
		case EEnemyState::Alive:
			ApplyKnockback();
			break;
	}
	if (InViewOfPlayer) {
		NotifyInViewOfPlayer();
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyCharacter::TakeDamage(float distance, float damage, FVector sourcePos, float launchForce, bool dropLoot) {
	if (EnemyState == EEnemyState::Dead) {
		return false;
	}
	//damage = damage * ((1/distance) * 1000);
	currentHealth -= damage;
	NotifyHealthBarWidget();
	NotifyDamageEnemy(damage, dropLoot);
	FVector knockbackDir = GetActorLocation() - sourcePos;
	knockbackDir.Z /= 4;
	Knockback(knockbackDir, launchForce);
	//LaunchCharacter((GetActorLocation()-sourcePos) * launchForce,true, true);
	if (currentHealth <= 0) {
		KillEnemy(dropLoot);
		return true;
	}
	return false;
}

bool AEnemyCharacter::KillEnemy(bool dropLoot)
{
	EnemyState = EEnemyState::Dead;
	EnemyKilled(dropLoot);
	return false;
}

void AEnemyCharacter::Knockback(FVector direction, float force)
{
	direction.Normalize();
	WasKnocked = true;
	KnockedDirection = direction;
	KnockSpeed = force;
}

bool AEnemyCharacter::IsInAttackRange(AActor* targetToAttack) {
	if (targetToAttack == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("NULL TARGET FOR ENEMEMY"));
		return false;
	}
	double distance = (GetActorLocation() - targetToAttack->GetActorLocation()).Size();
	if (distance < AttackRange) {
		if (AMyCharacter* targ = Cast<AMyCharacter>(targetToAttack)) {
			if (targ->IsPlayerDead) {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void AEnemyCharacter::SpawnPooledCharacter(FVector location, FRotator rotation, bool setTarget, FVector target)
{
	EnemyState = EEnemyState::Alive;
	currentHealth = MaxHealth;
	NotifyHealthBarWidget();
	Super::SpawnPooledCharacter(location, rotation, setTarget, target);
	EnemySpawned(setTarget, target);
}

void AEnemyCharacter::DespawnPooledCharacter()
{
	Super::DespawnPooledCharacter();
	KnockSpeed = 0;
	EnemyDespawned();
}



