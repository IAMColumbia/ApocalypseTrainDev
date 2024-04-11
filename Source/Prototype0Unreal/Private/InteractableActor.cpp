// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/InteractableActor.h"
#include <Components/SphereComponent.h>
#include <Characters/MyCharacter.h>
#include <Subsystems/PlayerManagerWSS.h>

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractableActor::DropObject(FVector directionToLaunch)
{
	state = EInteractableState::Dropped;
	OnDropped(directionToLaunch);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USphereComponent::StaticClass())) {
			USphereComponent* SphereCollisionComponent = Cast<USphereComponent>(component);
			if (SphereCollisionComponent && SphereCollisionComponent->ComponentHasTag("Trigger"))
			{
				trigger = SphereCollisionComponent;
				trigger->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnTriggerBeginOverlap);
				trigger->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnTriggerEndOverlap);
				
			}
		}
	}
}

void AInteractableActor::CheckForInteractPressed()
{
	if (wasInteracted) {
		return;
	}
	for(AMyCharacter* player : overlappingPlayers) {
		if (player->Interacted) {
			OnInteract(player);
		}
	}
}

void AInteractableActor::OnInteract(AMyCharacter* player)
{
	if (!player->Carrying) {
		player->PickupItem(this);
		state = EInteractableState::Carried;
		return;
	}
}

bool AInteractableActor::CanPlayerInteract(AMyCharacter* player)
{
	return true;
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for(AMyCharacter* c : GetWorld()->GetSubsystem<UPlayerManagerWSS>()->Players) {
		if (IsOverlappingActor(c) && !overlappingPlayers.Contains(c) && CanPlayerInteract(c)) {
			overlappingPlayers.Add(c);
			PlayersOverlapping();
		}
	}
	if (state == EInteractableState::Idle) {
		if (overlappingPlayers.Num() > 0) {
			CheckForInteractPressed();
		}
	}
	for (AMyCharacter* c : GetWorld()->GetSubsystem<UPlayerManagerWSS>()->Players) {
		if (!IsOverlappingActor(c) && overlappingPlayers.Contains(c)) {
			overlappingPlayers.Remove(c);
			if (overlappingPlayers.Num() <= 0) {
				NoPlayersOverlapping();
			}
		}
	}
}

//using overlaps can cause the overlappingPlayers collection to be modified while it is being iterated through. 
//Instead it is better to check if players are overlapping before and after we iterate through the collection and check for interaction.
// 
//(leaving this code here for portfolio documentation purposes.)
void AInteractableActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (AMyCharacter* player = Cast<AMyCharacter>(OtherActor)) {
			/*overlappingPlayers.Add(player);
			PlayersOverlapping();*/
		}
	}
}

void AInteractableActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (AMyCharacter* player = Cast<AMyCharacter>(OtherActor)) {
			/*overlappingPlayers.Remove(player);
			if (overlappingPlayers.Num() <= 0) {
				NoPlayersOverlapping();
			}*/
		}

	}
}

