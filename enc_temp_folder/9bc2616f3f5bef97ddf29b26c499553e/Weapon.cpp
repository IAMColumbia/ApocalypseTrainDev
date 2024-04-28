// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Characters/MyCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "Obstacle.h"
#include "Weapons/Projectiles/Projectile.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector AWeapon::GetOwnerActorLocation()
{
	if (OwnerCharacter != NULL) {
		return OwnerCharacter->GetActorLocation();
	}
	return GetActorLocation();
}

bool AWeapon::IsOwnerDead()
{
	if (OwnerCharacter->IsPlayerDead) {
		return true;
	}
	return false;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* sceneComponent = Cast<USceneComponent>(component);
			if (sceneComponent && sceneComponent->ComponentHasTag("BulletSpawn")) {
				BulletSpawn = sceneComponent;
			}
			if (sceneComponent && sceneComponent->ComponentHasTag("Laser")) {
				laser = sceneComponent;
			}
			if (sceneComponent && sceneComponent->ComponentHasTag("WeaponRotator")) {
				weaponRotator = sceneComponent;
			}
		}
		
	}
	Reload();
	CreateObjects();
}


void AWeapon::KilledEnemy()
{
	OwnerCharacter->OnEnemyKilled();
	GEngine->AddOnScreenDebugMessage(1, 3, OwnerCharacter->GetPlayerColor(), FString::Printf(TEXT("Killed %d"), OwnerCharacter->TotalKills));
}

FVector AWeapon::GetBeamEnd()
{
	FVector start = BulletSpawn->GetComponentLocation();

	//FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(characterMesh->GetRightVector(), 0.8);
	//FVector forward = OwnerCharacter->characterMesh->GetRightVector();
	FVector forward = BulletSpawn->GetForwardVector();
	forward.Z = 0;

	start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset), start.Z + (forward.Z * RayOffset));
	//maybe need to change end pos for randomness
	FVector end = start + forward * (RayLength/3);
	FHitResult hit;
	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerCharacter);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			return hit.Location;
		}

	}
	return end;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateReloadTime();
	UpdateAttackingState();
	if (IsWeaponClipping() && Equipped) {
		RaiseWeapon();
	}
	else if (!IsWeaponClipping() && Equipped) {
		LowerWeapon();
	}
}

void AWeapon::RaiseWeapon()
{
	if (!upright) {
		upright = true;
		HideLaser();
		FRotator CurrentRotation = weaponRotator->GetComponentRotation();
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll - 90.0f);
		weaponRotator->SetWorldRotation(NewRotation);
	}
}

void AWeapon::LowerWeapon()
{
	if (upright) {
		ShowLaser();
		upright = false;
		FRotator CurrentRotation = weaponRotator->GetComponentRotation();
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll + 90.0f);
		weaponRotator->SetWorldRotation(NewRotation);
	}
}


void AWeapon::WeaponEquipped()
{
	if (equipSound) {
		UGameplayStatics::PlaySoundAtLocation(this, equipSound, GetActorLocation(), 0.3f, 1+ FMath::RandRange(-0.1,0.1),0,attenuation);
	}

}

void AWeapon::GetRayVector(FVector*& rayStart, FVector*& rayEnd) {
	FVector start = GetActorLocation();
	FVector forward = BulletSpawn->GetForwardVector();
	forward.Z = 0;
	start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset), start.Z + (forward.Z * RayOffset));
	FVector end = start + forward * RayLength;
	rayStart = &start;
	rayEnd = &end;
}

void AWeapon::DetermineShotWithRay()
{
	if (BulletSpawn == NULL) {
		return;
	}
	FVector* start;
	FVector* end;
	GetRayVector(start, end);
	FHitResult hit;
	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerCharacter);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, *start, *end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				if (enemy->TakeDamage(hit.Distance, Damage - FMath::RandRange(0, ((int)(OwnerCharacter->DamageBuff / 2) + 3)) + OwnerCharacter->DamageBuff, GetActorLocation(), KnockbackForce, true)) {
					KilledEnemy();
				}
			}
			else {
				SpawnHitVFXType(hit);
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				obstacle->DamageObstacle(Damage);
			}
		}

	}
}

int AWeapon::OwnerPlayerIndex()
{
	return OwnerCharacter->PlayerIndex;
}

void AWeapon::UpdateReloadTime()
{
	currentReloadTime = GetWorld()->GetTimerManager().GetTimerElapsed(reloadTimerHandle);
	OwnerCharacter->NotifyReloadPercent(currentReloadTime, FireRate);
}

void AWeapon::UpdateAttackingState()
{
	if (prevAttackState && !Attacking) {
		NotifyStoppedShooting();
	}
	prevAttackState = Attacking;
	if (Attacking && Reloaded) {
		if (!IsWeaponClipping()) {
			NotifyFiredShot();
			ShootProjectile();
			Reloaded = false;
			GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, this, &AWeapon::Reload, FireRate, false);
		}
	}
	
}

void AWeapon::Reload()
{
	Reloaded = true;
}

void AWeapon::HideLaser()
{
	if (laser != NULL) {
		laser->SetVisibility(false);
	}
}

void AWeapon::ShowLaser()
{
	if (laser != NULL) {
		laser->SetVisibility(true);
	}
}

void AWeapon::CreateObjects()
{
	for (int i = 0; i < PoolCount; i++) {
		AProjectile* p = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileType));
		if (p != NULL) {
			p->InitializeProjectile(this);
			p->Despawn();
			objectPool.Enqueue(p);
		}
	}
}

void AWeapon::SpawnProjectile()
{
	if (!objectPool.IsEmpty()) {
		AProjectile* p;
		objectPool.Dequeue(p);
		p->Spawn();
		objectPool.Enqueue(p);
	}
}

bool AWeapon::IsWeaponClipping()
{
	if (BulletSpawn == NULL) {
		return false;
	}
	FVector start = BulletSpawn->GetComponentLocation();

	//FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(characterMesh->GetRightVector(), 0.8);
	//FVector forward = OwnerCharacter->characterMesh->GetRightVector();
	FVector forward = OwnerCharacter->GetActorLocation() - BulletSpawn->GetComponentLocation();
	forward.Z = 0;

	//start = FVector(start.X + (forward.X), start.Y + (forward.Y), start.Z + (forward.Z));
	//maybe need to change end pos for randomness
	FVector end = start + forward;

	FHitResult hit;

	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerCharacter);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		//NotifyFiredShot(OwnerCharacter->GetActorRightVector());

		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor() && !Cast<AEnemyCharacter>(hit.GetActor()) && !Cast<AObstacle>(hit.GetActor()) && !hit.GetActor()->ActorHasTag("EnemyTrap")) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			return true;
		}
	}
	return false;
}

USceneComponent* AWeapon::GetBulletSpawn()
{
	return BulletSpawn;
}


void AWeapon::Attack()
{
	Attacking = true;
}

void AWeapon::ShootProjectile() {
	OwnerCharacter->NotifyShot();
	SpawnProjectile();
	DetermineShotWithRay();
}

void AWeapon::SpawnHitVFXType(FHitResult hit)
{
	hitLocation = hit.Location;
	hitNormal = hit.Normal;
	if (hit.GetActor()->ActorHasTag("Wooden")) {
		SpawnBulletVFX(hitLocation, hitNormal, 1);
	}
	else {
		SpawnBulletVFX(hitLocation, hitNormal, 0);
	}
}

void AWeapon::EndAttack()
{
	Attacking = false;
}
