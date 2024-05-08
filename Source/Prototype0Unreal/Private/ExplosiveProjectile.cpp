// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/ExplosiveProjectile.h"
#include "Components/SphereComponent.h"
#include "Weapons/Weapon.h"

// Sets default values
AExplosiveProjectile::AExplosiveProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveProjectile::Explode()
{
	float radius = 200;
	USphereComponent* explosionHitCheck = Cast<USphereComponent>(GetWorld()->SpawnActorAbsolute(USphereComponent::StaticClass(), GetActorTransform()));
	explosionHitCheck->InitSphereRadius(radius);
}

float AExplosiveProjectile::GetDamage()
{
	return ownerWeapon->GetDamage();
}

void AExplosiveProjectile::KilledEnemy()
{
	ownerWeapon->KilledEnemy();
}


