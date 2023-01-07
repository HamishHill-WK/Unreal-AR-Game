// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlaceableActor.h"
#include "HelloARManager.h"
#include "GameController.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

//Forward Declarations
class APlaceableActor;

UCLASS()
class UE5_AR_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:

	FTimerHandle Ticker;

	AHelloARManager* ARmanager;

	AGameController* GameController;

public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SpawnInitialActors();

	 UPROPERTY(Category="Placeable",EditAnywhere,BlueprintReadWrite)
	 TSubclassOf<APlaceableActor> PlacableToSpawn;

	 UPROPERTY(Category="Placeable",EditAnywhere,BlueprintReadWrite)
	 TSubclassOf<AActor> Plane;

	 int PinCount; //for debug to track number of pins 
};
