// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomGameState.h"

ACustomGameState::ACustomGameState() :
	Bullets(10)
{
	StartSpawned = false;
	EndSpawned = false;
	EnemySpawned = false;
	CubeSelected = false;
	ECurrentGamePhase = idle;
}
