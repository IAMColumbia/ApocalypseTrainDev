// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ProjectileShooter.h"
#include "Weapon.h"

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveProjectile();
}

void AProjectile::Spawn() {
	if (Cast<AWeapon>(Owner))
	{
		storedHitLocation = Cast<AWeapon>(Owner)->hitLocation;
		storedHitNormal = Cast<AWeapon>(Owner)->hitNormal;
	}
		Despawning = false;
		SetActorRotation(spawnPoint->GetComponentRotation());
		SetActorLocation(spawnPoint->GetComponentLocation());
		APooledActor::Spawn();
		GetWorld()->GetTimerManager().SetTimer(lifetimeHandle, this, &APooledActor::Despawn, Lifetime, false);
		Active = true;
}

void AProjectile::SpawnWithRotation(FRotator rot)
{
	SetActorRotation(rot);
	SetActorLocation(spawnPoint->GetComponentLocation());
	APooledActor::Spawn();
	GetWorld()->GetTimerManager().SetTimer(lifetimeHandle, this, &APooledActor::Despawn, Lifetime, false);
	Active = true;
}

void AProjectile::Despawn()
{
	Active = false;
	APooledActor::Despawn();
}

void AProjectile::InitializeProjectile(IProjectileShooter* owner)
{
	Owner = owner;
	spawnPoint = Owner->GetBulletSpawn();
}



FVector AProjectile::GetOwnerCharacterLocation()
{
	if (Owner != NULL) {
		return Owner->GetOwnerActorLocation();
	}
	//this shouldnt be getting called, just here so editor doesnt crash
	return GetActorLocation();
}

void AProjectile::MoveProjectile()
{
	FVector location = GetActorLocation();
	location += GetActorForwardVector() * ProjectileSpeed;
	SetActorLocation(location);
}
