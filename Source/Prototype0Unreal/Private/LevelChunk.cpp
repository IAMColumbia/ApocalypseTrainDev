// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/LevelChunk.h"

// Sets default values
ALevelChunk::ALevelChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelChunk::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* Component : components)
	{
		if (Component->IsA(UStaticMeshComponent::StaticClass()))
		{
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(Component);
			if (mesh && mesh->ComponentHasTag("ChunkBase")) {
				chunkBase = mesh;
			}
		}
	}
}

// Called every frame
void ALevelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChunk::GetLength_Implementation(UPARAM(DisplayName = "Chunk Base Length") float& length_out)
{
	if (chunkBase != NULL) {
		length_out = chunkBase->GetComponentScale().Y * 100;
	}
	else {
		length_out = 0.0f;
	}
}

FVector ALevelChunk::GetScale()
{
	if (chunkBase == NULL) {
		return FVector(0);
	}
	return chunkBase->GetComponentScale() * 50;
}

