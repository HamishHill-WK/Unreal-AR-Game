// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class UE5_AR_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Moving = false; 
	FVector TargetLoc;
	FVector CurrentLoc;
	FVector CurrentRot;

	float TravelTime;
	float MaxTravelTime = 2.5f;

	UFUNCTION(BlueprintCallable, Category = "myCategory")
		void BulletDie();
};
