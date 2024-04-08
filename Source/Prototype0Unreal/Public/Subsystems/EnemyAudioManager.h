// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAudioManager.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AEnemyAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyAudioManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int activeDamages;
	USoundBase* getRandomSound(TArray<USoundBase*> files);
	UFUNCTION()
	void DamageVoiceFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback")
	float pitchVariation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AudioFiles")
	TArray<USoundBase*> AttackFiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AudioFiles")
	TArray<USoundBase*> DamagedFiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AudioFiles")
	TArray<USoundBase*> DiedFiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mixing")
	USoundAttenuation* attenuationSettings;

	UFUNCTION(BlueprintCallable)
	void PlayAttackSound(FVector location);

	UFUNCTION(BlueprintCallable)
	void PlayDamageSound(FVector pos);

	UFUNCTION(BlueprintCallable)
	void PlayDeathSound(FVector pos);

	void InitilizeComponents();
	UAudioComponent* PlaySound(USoundBase* soundToPlay, FVector pos);
	TQueue<UAudioComponent*> availableAttackVoiceComponents;
	TQueue<UAudioComponent*> availableDamageVoiceComponents;
	TQueue<UAudioComponent*> availableDeathVoiceComponents;

};
