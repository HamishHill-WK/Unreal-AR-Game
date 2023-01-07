// Fill out your copyright notice in the Description page of Project Settings.
#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	CurrentLoc = this->GetActorLocation();
	CurrentRot = this->GetActorForwardVector();
	FVector movement = CurrentLoc + (CurrentRot * 10);
	SetActorLocation(movement);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TravelTime += DeltaTime;
	CurrentLoc = this->GetActorLocation();
	CurrentRot = this->GetActorForwardVector();
	FVector movement = CurrentLoc + (CurrentRot * 1000 * DeltaTime);

	SetActorLocation(movement);

	if (TravelTime >= MaxTravelTime)
		BulletDie();
}

void ABullet::BulletDie()
{
	Destroy();
}
