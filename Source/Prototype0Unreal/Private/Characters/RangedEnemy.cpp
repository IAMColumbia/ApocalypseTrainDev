// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/RangedEnemy.h"
#include "Subsystems/PlayerManagerWSS.h"
#include "Weapons/Projectiles/EnemyProjectile.h"
#include "Characters/MyCharacter.h"

// Sets default values
ARangedEnemy::ARangedEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
    playerManager = GetWorld()->GetSubsystem<UPlayerManagerWSS>();
    GetWorld()->GetTimerManager().SetTimer(shootTimer, this, &ARangedEnemy::SetIsAttacking, fireRate + FMath::RandRange(-randomFireRateDeviation, randomFireRateDeviation), false);
    if (playerManager->PlayerHasSpawned()) {
        currentTarget = playerManager->GetRandomPlayer();
    }
    TSet<UActorComponent*> components = GetComponents();
    for (UActorComponent* component : components) {
        if (component->IsA(USceneComponent::StaticClass())) {
            USceneComponent* sceneComponent = Cast<USceneComponent>(component);
            if (sceneComponent && sceneComponent->ComponentHasTag("ProjectileSpawn")) {
                projectileSpawn = sceneComponent;
            }
        }

    }
}

FVector ARangedEnemy::getTargetLocation()
{
    if (currentTarget == NULL) {
        return FVector(0, 0, 0);
    }
    return currentTarget->GetActorLocation();
}

void ARangedEnemy::FireShot()
{
    if (currentTarget != NULL) {
        AEnemyProjectile* a = Cast<AEnemyProjectile>(GetWorld()->SpawnActorAbsolute(enemyProjectile, projectileSpawn->GetComponentTransform()));
        a->Launch(GetLaunchVector(projectileSpawn->GetComponentLocation(), getTargetLocation())+ FVector(0,0,1000));
    }
    GetWorld()->GetTimerManager().SetTimer(shootTimer, this, &ARangedEnemy::SetIsAttacking, fireRate + FMath::RandRange(-randomFireRateDeviation, randomFireRateDeviation), false);
    if (playerManager->PlayerHasSpawned()) {
        currentTarget = playerManager->GetRandomPlayer();
    }
}

void ARangedEnemy::SetIsAttacking()
{
    IsAttacking = true;
}

// Called every frame
void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangedEnemy::DamageObstacle(float damage)
{
    currentHealth -= FGenericPlatformMath::Abs(damage);
    NotifyDamageObstacle(damage);
    if (currentHealth <= 0) {
        GetWorld()->GetTimerManager().ClearTimer(shootTimer);
        ObstacleDestroyed();
    }
}
//void ARangedEnemy::SpawnPooledCharacter(FVector location, FRotator rotation, bool setTarget, FVector target)
//{
//    Super::SpawnPooledCharacter(location, rotation, setTarget, target);
//    GetWorld()->GetTimerManager().SetTimer(shootTimer, this, &ARangedEnemy::FireShot, fireRate, false);
//}
//
//void ARangedEnemy::DespawnPooledCharacter()
//{
//    Super::DespawnPooledCharacter();
//    GetWorld()->GetTimerManager().ClearTimer(shootTimer);
//}


// Function to calculate launch velocity
float ARangedEnemy::CalculateLaunchVelocity(float gravity, float distance, float angle)
{
    return sqrtf((gravity * distance) / sinf(2 * angle));
}

// Function to calculate launch angle
float ARangedEnemy::CalculateLaunchAngle(float gravity, float distance, float initialVelocity)
{
    return 0.5f * asinf((gravity * distance) / (initialVelocity * initialVelocity));
}

// Example function to launch an object
FVector ARangedEnemy::GetLaunchVector(FVector StartPosition, FVector TargetPosition)
{
    // Calculate the distance between start and target positions
    FVector LaunchDirection = (TargetPosition - StartPosition).GetSafeNormal();
    float Distance = (TargetPosition - StartPosition).Size();

    // Set the gravity (in Unreal units, approximately 980 cm/s^2)
    float Gravity = 980.0f;

    // Calculate the initial velocity
    float InitialVelocity = CalculateLaunchVelocity(Gravity, Distance, PI / 4);
    
    // Calculate the launch angle
    float LaunchAngle = CalculateLaunchAngle(Gravity, Distance, InitialVelocity);

    // Set the velocity of the object
    return  LaunchDirection * InitialVelocity;
}
