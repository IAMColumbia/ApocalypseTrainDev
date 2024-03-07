// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Swivle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Swivle"));

	RootComponent = Collider;
	Base->SetupAttachment(Collider);
	Swivle->SetupAttachment(Base);


	TraceCount = 20;

	ConeAngle = 45.0f;
	ConeLength = 1000.0f;
	
	Reloaded = true;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	TraceGap = ConeAngle / TraceCount;
	ConeDirection = GetActorForwardVector();

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* sceneComponent = Cast<USceneComponent>(component);
			if (sceneComponent && sceneComponent->ComponentHasTag("BulletSpawn")) {
				BulletSpawn = sceneComponent;
			}
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, BulletSpawn->GetName());	
	CreateObjects();
	Reload();
}

// Called every frame 
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ConeOrigin = GetActorLocation();
	VisionSweep();

}

void ATurret::VisionSweep()
{
	float StartAngle = -ConeAngle / 2;
		
	for (int32 i = 0; i < TraceCount; ++i)
	{
		//FVector TraceDir = -(FVector(FMath::Cos(FMath::DegreesToRadians(TraceGap * i)), FMath::Sin(FMath::DegreesToRadians(TraceGap * i)), 0.0f));
		//FVector TraceEnd = ConeOrigin + (TraceDir * ConeLength);

		FVector RotatedDir = ConeDirection.RotateAngleAxis(StartAngle + TraceGap * i, FVector(0, 0, 1));
		FVector TraceEnd = ConeOrigin + (RotatedDir * ConeLength);

		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("Vision Trace")), true, this); //Set complex collisions to true as I don't know how you guys tackle your enemy registration
		GetWorld()->LineTraceSingleByChannel(HitResult, ConeOrigin, TraceEnd, ECC_Pawn, TraceParams);

		if (HitResult.bBlockingHit)
		{
			AActor* TargetActor = HitResult.GetActor();
			if (IsEnemy(TargetActor))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("TURRET SEES ZOMBIE!"));	
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
				TargetRotation.Pitch = 0;
				Swivle->SetWorldRotation(TargetRotation);
				
				if (Reloaded)
				{
					ShootProjectile();
				}
			}
		}

		DrawDebugLine(GetWorld(), ConeOrigin, TraceEnd, FColor::Red, false, 0.1f, 0, 2);
	}
}

	bool ATurret::IsEnemy(AActor* Actor)
	{
		return Actor && Actor->IsA(AEnemyCharacter::StaticClass());
	}

	void ATurret::CreateObjects()
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

	void ATurret::SpawnProjectile()
	{
		if (!objectPool.IsEmpty()) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SHOOOOOOOT!"));
			AProjectile* p;
			objectPool.Dequeue(p);
			p->Spawn();
			objectPool.Enqueue(p);
		}
	}

	void ATurret::ShootProjectile()
	{
		Reloaded = false;
		SpawnProjectile();
		NotifyFiredShot();
		Ray();
		GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, this, &ATurret::Reload, FireRate, false);
	}

	FVector ATurret::GetOwnerActorLocation()
	{
		return GetActorLocation();
	}

	USceneComponent* ATurret::GetBulletSpawn()
	{
		return BulletSpawn;
	}


void ATurret::Reload()
{
	Reloaded = true;
}

void ATurret::Ray()
{
	if (BulletSpawn == NULL) {
		return;
	}
	FVector start = GetActorLocation();

	//FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(characterMesh->GetRightVector(), 0.8);
	//FVector forward = OwnerCharacter->characterMesh->GetRightVector();
	FVector forward = BulletSpawn->GetForwardVector();
	forward.Z = 0;

	start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset ), start.Z + (forward.Z * RayOffset));
	//maybe need to change end pos for randomness
	FVector end = start + forward * RayLength;

	FHitResult hit;


	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		//NotifyFiredShot(OwnerCharacter->GetActorRightVector());
		
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				enemy->TakeDamage(hit.Distance, Damage, GetActorLocation(), KnockbackForce, true);
			}
			/*else {
				SpawnHitVFXType(hit);
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				obstacle->DamageObstacle(Damage);
			}
			*/
		}

	}
}

