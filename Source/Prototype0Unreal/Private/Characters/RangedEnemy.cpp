// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RangedEnemy.h"

// Sets default values
ARangedEnemy::ARangedEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Function to calculate launch velocity
float ARangedEnemy::CalculateLaunchVelocity(float gravity, float distance, float angle)
{
    return sqrtf((gravity * distance) / sinf(2 * angle));
}

// Function to calculate launch angle
float ARangedEnemy::CalculateLaunchAngle(float gravity, float distance, float initialVelocity)
{
    return 0.5f * asinf((gravity * distance) / (initialVelocity * initialVelocity));
}

// Example function to launch an object
FVector ARangedEnemy::GetLaunchVector(FVector StartPosition, FVector TargetPosition)
{
    // Calculate the distance between start and target positions
    FVector LaunchDirection = (TargetPosition - StartPosition).GetSafeNormal();
    float Distance = (TargetPosition - StartPosition).Size();

    // Set the gravity (in Unreal units, approximately 980 cm/s^2)
    float Gravity = 980.0f;

    // Calculate the initial velocity
    float InitialVelocity = CalculateLaunchVelocity(Gravity, Distance, PI / 4);
    
    // Calculate the launch angle
    float LaunchAngle = CalculateLaunchAngle(Gravity, Distance, InitialVelocity);

    // Set the velocity of the object
    return  LaunchDirection * InitialVelocity;
}
