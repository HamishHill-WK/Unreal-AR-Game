// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomGameMode.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ACustomGameMode::ACustomGameMode()
{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	// Set the default pawn and gamestate to be our custom pawn and gamestate programatically
	DefaultPawnClass = ACustomARPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();
}

void ACustomGameMode::StartPlay() 
{
	SpawnInitialActors();
	Super::StartPlay();
}

void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACustomGameMode::SpawnInitialActors()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(0, 0, 0);
	// Spawn an instance of the HelloARManager class
	ARmanager = GetWorld()->SpawnActor<AHelloARManager>(MyLoc, MyRot, SpawnInfo);
	GameController = GetWorld()->SpawnActor<AGameController>(MyLoc, MyRot, SpawnInfo);
	GameController->SetGameMode(GetWorld()->GetAuthGameMode());
	GameController->SetARmanager(ARmanager);
}