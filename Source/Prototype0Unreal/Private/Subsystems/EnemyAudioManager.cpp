// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/EnemyAudioManager.h"
#include "Components/AudioComponent.h"

// Sets default values
AEnemyAudioManager::AEnemyAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitilizeComponents();
}

// Called when the game starts or when spawned
void AEnemyAudioManager::BeginPlay()
{
	Super::BeginPlay();
}

USoundBase* AEnemyAudioManager::getRandomSound(TArray<USoundBase*> files)
{
	return files[FMath::RandRange(0, files.Num() - 1)];
}

void AEnemyAudioManager::DamageVoiceFinished()
{

}

// Called every frame
void AEnemyAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAudioManager::PlayAttackSound(FVector location)
{
	if (AttackFiles.Num() <= 0) {
		return;
	}
	if ((*availableAttackVoiceComponents.Peek())->IsPlaying()) {
		return;
	}
	PlaySound(getRandomSound(AttackFiles), location);
}

void AEnemyAudioManager::PlayDamageSound(FVector pos)
{
	if (DamagedFiles.Num() <= 0) {
		return;
	}
	if ((*availableDamageVoiceComponents.Peek())->IsPlaying()) {
		return;
	}
	UAudioComponent* c = PlaySound(getRandomSound(DamagedFiles), pos);
	//c->OnAudioFinished.AddDynamic(this, &AEnemyAudioManager::DamageVoiceFinished);
}

void AEnemyAudioManager::PlayDeathSound(FVector pos)
{
	if (DiedFiles.Num() <= 0) {
		return;
	}
	if ((*availableDeathVoiceComponents.Peek())->IsPlaying()) {
		return;
	}
	UAudioComponent* c = PlaySound(getRandomSound(DiedFiles), pos);
}

UAudioComponent* AEnemyAudioManager::PlaySound(USoundBase* soundToPlay, FVector pos)
{
	if (availableDamageVoiceComponents.IsEmpty()) {
		return NULL;
	}
	UAudioComponent* comp;
	availableDamageVoiceComponents.Dequeue(comp);
	comp->SetSound(soundToPlay);
	comp->SetWorldLocation(pos);
	float RandomPitchMultiplier = FMath::RandRange(1-pitchVariation, 1+pitchVariation);
	comp->SetPitchMultiplier(RandomPitchMultiplier);
	comp->Play();
	availableDamageVoiceComponents.Enqueue(comp);
	return comp;
}

void AEnemyAudioManager::InitilizeComponents()
{
	int maxvoices = 10;
	for (int i = 0; i < maxvoices; i++) {
		FString AudioComponentName = FString::Printf(TEXT("DamagedComponent %d"), i);
		UAudioComponent* comp = CreateDefaultSubobject<UAudioComponent>(*AudioComponentName, true);
		if (comp) {
			//comp->SetupAttachment(RootComponent);
			comp->bAutoActivate = false;
			comp->bAutoDestroy = false;
			comp->bAllowSpatialization = true;
			comp->bOverrideAttenuation = true;
			comp->AttenuationSettings = attenuationSettings;
			availableDamageVoiceComponents.Enqueue(comp);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to create audio component %s"), *AudioComponentName);
		}
	}
	for (int i = 0; i < maxvoices; i++) {
		FString AudioComponentName = FString::Printf(TEXT("DeathAudioComponent %d"), i);
		UAudioComponent* comp = CreateDefaultSubobject<UAudioComponent>(*AudioComponentName, true);
		if (comp) {
			//comp->SetupAttachment(RootComponent);
			comp->bAutoActivate = false;
			comp->bAutoDestroy = false;
			comp->bAllowSpatialization = true;
			comp->bOverrideAttenuation = true;
			comp->AttenuationSettings = attenuationSettings;
			availableDeathVoiceComponents.Enqueue(comp);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to create audio component %s"), *AudioComponentName);
		}
	}
	for (int i = 0; i < maxvoices; i++) {
		FString AudioComponentName = FString::Printf(TEXT("AttackAudioComponent %d"), i);
		UAudioComponent* comp = CreateDefaultSubobject<UAudioComponent>(*AudioComponentName, true);
		if (comp) {
			//comp->SetupAttachment(RootComponent);
			comp->bAutoActivate = false;
			comp->bAutoDestroy = false;
			comp->bAllowSpatialization = true;
			comp->bOverrideAttenuation = true;
			comp->AttenuationSettings = attenuationSettings;
			availableAttackVoiceComponents.Enqueue(comp);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to create audio component %s"), *AudioComponentName);
		}
	}

}

