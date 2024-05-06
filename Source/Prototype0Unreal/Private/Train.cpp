// Fill out your copyright notice in the Description page of Project Settings.



#include "Train.h"
#include "Subsystems/GameManagerWSS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/EnemyCharacter.h"
#include "Subsystems/PlayerManagerWSS.h"
#include "Obstacle.h"
#include "Characters/MyCharacter.h"
#include <Internationalization/Text.h>
#include "Interactables/InteractableActor.h"

ATrain::ATrain()
{
	PrimaryActorTick.bCanEverTick = true;
	CanMove = false;
	Fuel = MaxFuel;
	currentTrainState = ETrainState::starting;
	GraceTimeRemaining = GameOverGraceTime;
}

void ATrain::IncreaseDifficulty()
{
	burnRate += burnRateIncrease;
	MaxTrainSpeed += trainSpeedIncrease;
	DecelerationRate += trainSpeedIncrease;
}

void ATrain::StartTrain()
{
	if (HasFuel() && currentTrainState != ETrainState::accelerating) {
		SetTrainState(ETrainState::accelerating);
	}
}

void ATrain::StopTrain()
{
	if (currentTrainState == ETrainState::accelerating) {
		SetTrainState(ETrainState::decelerating);
	}
}

void ATrain::MovementUpdate()
{
	switch (currentTrainState) {
		case ETrainState::stopped:
			if (!Stopped && !countingDownGameOver) {
				SetTrainState(ETrainState::accelerating);
			}
			break;
		case ETrainState::starting:
			if (currentTrainSpeed < tutorialMaxTrainSpeed) {
				currentTrainSpeed += AccelerationRate;
			}
			if (GetActorLocation().Y > TutorialEnd) {
				SetTrainState(ETrainState::accelerating);
				GetWorld()->GetSubsystem<UGameManagerWSS>()->OnTutorialExit();
			}
			break;
		case ETrainState::accelerating:
			if (currentTrainSpeed < MaxTrainSpeed) {
				currentTrainSpeed += AccelerationRate;
			}
			break;
		case ETrainState::decelerating:
			if (isReversing) {
				if (currentTrainSpeed < 0) {
					currentTrainSpeed += DecelerationRate;
				}
				else {
					isReversing = false;
				}
			}
			else {
				if (currentTrainSpeed > 0) {
					currentTrainSpeed -= DecelerationRate;
				}
				else {
					SetTrainState(ETrainState::stopped);
					currentTrainSpeed = 0;
				}
			}
			break;
		default:
			break;
	}
	if (currentTrainState != ETrainState::stopped && currentTrainState != ETrainState::decelerating) {
		BurnFuel();
	}
}

bool ATrain::HasFuel()
{
	if (Fuel > 0) {
		return true;
	}
	return false;
}

FVector ATrain::GetDeadPlayerHolderLocation()
{
	if (deadPlayerHolder != NULL) {
		return deadPlayerHolder->GetComponentLocation() + FVector( 0,0,100);
	}
	return FVector(0, 0, 0);
}

void ATrain::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->train = this;
	targetYPos = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetNextTargetLocation();
	TotalMeters = 0;
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* Component : components)
	{
		if (Component->IsA(UBoxComponent::StaticClass()))
		{
			UBoxComponent* BoxCollisionComponent = Cast<UBoxComponent>(Component);
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("FuelDeposit"))
			{
				fuelDeposit = BoxCollisionComponent;
			}
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("Plow"))
			{
				plow = BoxCollisionComponent;
				plow->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnPlowBeginOverlap);
			}
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("startBox"))
			{
				leverBox = BoxCollisionComponent;
			}
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("DeadPlayerHolder"))
			{
				deadPlayerHolder = BoxCollisionComponent;
			}
		}
		if (Component->IsA(UStaticMeshComponent::StaticClass())) {
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(Component);
			if (mesh && mesh->ComponentHasTag("StartLever")) {
				startLeverMesh = mesh;
			}
		}
		if (Component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* point = Cast<USceneComponent>(Component);
			if (point && point->ComponentHasTag("EncounterSpawnPoint")) {
				EncounterSpawnPoints.Add(point);
			}
		}
	}
	ShouldZoomCamera = true;
	
}

void ATrain::PlayerJoined()
{
	
	burnRate += burnRateDifficultyScaling;
}

bool ATrain::FuelIsCritical()
{
	if (FuelState == EFuelState::critical) {
		return true;
	}
	return false;
}

void ATrain::TrainCanMove()
{
	CanMove = true;
	NotifyTrainStartGame();
}

void ATrain::IncrementTotalMeters()
{
	if (CanMove) {
		TotalMeters++;
		NotifyIncrementMeters(TotalMeters);
	}
}

void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanMove) {
		MovementUpdate();
		currentLocation = GetActorLocation();
		currentLocation += FVector(0, 1, 0) * currentTrainSpeed * DeltaTime;
		SetActorLocation(currentLocation, true);
	}
	if (currentLocation.Y >= targetYPos) {
		GetWorld()->GetSubsystem<UGameManagerWSS>()->SpawnNewChunk();
	}
	UpdateFuelState();
}

bool ATrain::IsOverlappingFuelBox(FVector actorPos) {
	if (UKismetMathLibrary::IsPointInBox(actorPos, fuelDeposit->GetComponentLocation(), fuelDeposit->GetScaledBoxExtent())) {
		return true;
	}
	return false;
}

bool ATrain::IsOverlappingLeverBox(FVector actorPos) {
	if (UKismetMathLibrary::IsPointInBox(actorPos, leverBox->GetComponentLocation(), leverBox->GetScaledBoxExtent())) {
		return true;
	}
	return false;
	
}

void ATrain::ToggleTrainState()
{
	switch (leverState) {
	case ELeverState::stop:
		leverState = ELeverState::move;
		break;
	case ELeverState::move:
		leverState = ELeverState::stop;
		break;
	}
	LeverStateChanged();
}

void ATrain::StartHordeEncounter()
{
	ShouldZoomCamera = false;
	Stopped = true;
	StopTrain();
}

bool ATrain::AddFuel() {
	if (!CanMove) {
		GetWorld()->GetTimerManager().SetTimer(startTimerHandle, this, &ATrain::TrainCanMove, 1.0, false);
	}
	if (Fuel + 1 <= MaxFuel) {
		Fuel++;
		NotifyFuelAdded();
		return true;
	}
	else {
		return false;
	}
}

void ATrain::BurnFuel() {
	Fuel -= burnRate;
	if (Fuel < 0) {
		Fuel = 0;
		SetTrainState(ETrainState::decelerating);
	}
}

void ATrain::UpdateFuelState()
{
	if (Fuel > MaxFuel / CriticalLevel) {
		FuelState = EFuelState::normal;
		NotifyFuelNormal();
	}
	else if (Fuel <= MaxFuel / CriticalLevel && !countingDownGameOver) {
		FuelState = EFuelState::critical;
		NotifyFuelCritical();
		//notify danger
	}
	if (!HasFuel()) {
		//notify failed
		if (!countingDownGameOver && !GetWorld()->GetSubsystem<UGameManagerWSS>()->gameEnded) {
			countingDownGameOver = true;
			GraceTimeRemaining = GameOverGraceTime;
			GetWorld()->GetTimerManager().SetTimer(countdown, this, &ATrain::DecrementGameOverCounter, 1, true);
		}
	}
	if (countingDownGameOver) {
		if (HasFuel()) {
			GetWorld()->GetTimerManager().ClearTimer(countdown);
			countingDownGameOver = false;
		}
	}
}

FVector ATrain::GetRandomEncounterSpawnPos()
{
	if (EncounterSpawnPoints.Num() > 0) {
		int num = FMath::RandRange(0, EncounterSpawnPoints.Num() - 1);
		return EncounterSpawnPoints[num]->GetComponentLocation();
	}
	FVector loc = FVector(-40, 0, 250);
	loc.Y = GetBackBound() + 500;
	return loc;
}

void ATrain::DecrementGameOverCounter()
{
	NotifyGameOverCounter();
	if (GraceTimeRemaining <= 0 && !HasFuel()) {
		GetWorld()->GetTimerManager().ClearTimer(countdown);
		GetWorld()->GetSubsystem<UGameManagerWSS>()->GameOver(0);
	}
	GraceTimeRemaining--;
}

bool ATrain::IsTrainStopped()
{
	if (currentTrainState == ETrainState::stopped) {
		return true;
	}
	return false;
}

FVector ATrain::GetRandomRespawnPos() {
	int posChoice = UKismetMathLibrary::RandomInteger(4);
	switch (posChoice) {
		case 0:
			return GetActorLocation() + FVector(200, -100, 0);
		case 1:
			return GetActorLocation() + FVector(-200, -100, 0);
		case 2:
			return GetActorLocation() + FVector(200, -400, 0);
		case 3:
			return GetActorLocation() + FVector(-200, -400, 0);
		default:
			return GetActorLocation() + FVector(-200, -100, 0);
	}
}

float ATrain::GetBackBound()
{
	return GetActorLocation().Y - BackBound;
}

float ATrain::GetFrontBound()
{
	return BackBound + GetActorLocation().Y;
}

void ATrain::OnPlowBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (currentTrainSpeed <= minnimumDamagingSpeed) {
		return;
	}
	if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor)) {
		
		enemy->TakeDamage(0, currentTrainSpeed * damageMultiplier, GetActorLocation(), currentTrainSpeed * launchMultiplier, true);
		NotifyTrainHitEnemy();
	}
	if (AMyCharacter* player = Cast<AMyCharacter>(OtherActor)) {
		player->TakeDamage(playerDamage, 4);
		player->LaunchCharacter(GetActorRightVector() * playerLaunchForce, true, true);
	}
	if (OtherActor->Tags.Contains("Fuel")) {
		if (AInteractableActor* f = Cast<AInteractableActor>(OtherActor)) {
			if (f->state != EInteractableState::Carried) {
				OtherActor->Destroy();
			}
		}
	}
	if (OtherActor->Tags.Contains("Obstacle")) {
		currentTrainSpeed *= -1;
		isReversing = true;
		ToggleTrainState();
		NotifyTrainHitObstacle();
		if (AObstacle* obstacle = Cast<AObstacle>(OtherActor)) {

			obstacle->DamageObstacle(obstacleDamage);
		}
		Fuel -= CollisionFuelLoss;
	}
}

FVector ATrain::GetRespawnPos(int PlayerIndex) {
	switch (PlayerIndex) {
		case 0:
			return player0RespawnPos + GetActorLocation();
		case 1:
			return player1RespawnPos + GetActorLocation();
		case 2:
			return player2RespawnPos + GetActorLocation();
		case 3:
			return player3RespawnPos + GetActorLocation();
		default:
			return player1RespawnPos + GetActorLocation();
	}
}

void ATrain::SetTrainState(ETrainState stateToSet)
{
	switch (stateToSet) {
		case ETrainState::stopped:
			NotifyTrainStop();
			GetWorld()->GetSubsystem<UGameManagerWSS>()->OnTrainStopped();
			break;
		case ETrainState::accelerating:
			NotifyTrainStart();
			GetWorld()->GetSubsystem<UGameManagerWSS>()->OnTrainAccelerating();
			break;
		case ETrainState::decelerating:
			break;
	}
	currentTrainState = stateToSet;
}
