// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChunk.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API ALevelChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetLength(UPARAM(DisplayName = "Chunk Base Length") float& length_out);
	UFUNCTION(BlueprintCallable)
	virtual FVector GetScale();

	UStaticMeshComponent* chunkBase;


};
