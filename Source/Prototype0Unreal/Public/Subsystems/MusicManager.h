// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AMusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayStartNotify();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPickupNotify();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMainNotify();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEncounterNotify();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGameOverNotify();

};
