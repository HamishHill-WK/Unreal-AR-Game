// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameState.generated.h"

UENUM() 
enum EGamePhase { idle , creator , player };

UCLASS()
class UE5_AR_API ACustomGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACustomGameState();
	~ACustomGameState() = default;
	
	int32 Bullets;

	bool StartSpawned;
	bool EndSpawned;
	bool EnemySpawned;
	bool CubeSelected;

	EGamePhase ECurrentGamePhase;
	EGamePhase GetCustomGamePhase() const { return ECurrentGamePhase; }
};
