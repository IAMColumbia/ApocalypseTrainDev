// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuManager.generated.h"




UCLASS()
class PROTOTYPE0UNREAL_API AMenuManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GameOver(int GameOverReason);

	UFUNCTION(BlueprintImplementableEvent)
	void GameOverNotify(int GameOverReason);

};
