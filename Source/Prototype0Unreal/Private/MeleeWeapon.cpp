// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Obstacle.h"
#include "EnemyCharacter.h"
#include "MyCharacter.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* Component : components)
	{
		if (Component->IsA(UBoxComponent::StaticClass()))
		{
			UBoxComponent* BoxCollisionComponent = Cast<UBoxComponent>(Component);
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("Hitbox"))
			{
				box = BoxCollisionComponent;
				box->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnWeaponBeginOverlap);
			}
		}
	}
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanDamage) {
		if (AObstacle* obstacle = Cast<AObstacle>(OtherActor)) {

			obstacle->DamageObstacle(ObstacleDamage);
		}
		if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor)) {
			if (enemy->TakeDamage(0, EnemyDamage, GetActorLocation(), KnockbackForce, true)) {
				if (OwnerCharacter != NULL) {
					OwnerCharacter->OnEnemyKilled();
				}
			}
		}
	}
}

