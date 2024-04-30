// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MenuManager.h"
#include "Subsystems/GameManagerWSS.h"



// Sets default values
AMenuManager::AMenuManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMenuManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->menuManager = this;
}

// Called every frame
void AMenuManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMenuManager::GameOver(int GameOverReason)
{
	float delayTime = 8;
	/*if (GameOverReason == 0) {
		GEngine->AddOnScreenDebugMessage(-1, delayTime, FColor::Red, FString::Printf(TEXT("Game Over \nTrain Out Of Fuel")), true, FVector2D(10, 10));
	}
	if (GameOverReason == 1) {
		GEngine->AddOnScreenDebugMessage(-1, delayTime, FColor::Red, FString::Printf(TEXT("Game Over \nAll Players Died")), true, FVector2D(10, 10));
	}*/

	GameOverNotify(GameOverReason);
	//FTimerHandle resetTimer;
	//GetWorld()->GetTimerManager().SetTimer(resetTimer, this, &UGameManagerWSS::RestartGame, delayTime, true);
}

