// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ExplosiveLauncher.h"
#include "Characters/MyCharacter.h"
#include "Weapons/Projectiles/ExplosiveProjectile.h"



void AExplosiveLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void AExplosiveLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AExplosiveLauncher::ShootProjectile()
{
	FVector LaunchDirection = BulletSpawn->GetForwardVector();
	AExplosiveProjectile* a = Cast<AExplosiveProjectile>(GetWorld()->SpawnActorAbsolute(ExplosiveProjectile, BulletSpawn->GetComponentTransform()));
	a->ownerWeapon = this;
	a->Launch(LaunchDirection);
	a->FiredPlayerIndex = OwnerPlayerIndex();
}
