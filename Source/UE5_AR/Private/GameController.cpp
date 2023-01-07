// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "GameFramework/Actor.h"
#include "CustomGameState.h"
#include "ARPlaneActor.h"
#include "CustomGameMode.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlaceableActor.h"

// Sets default values
AGameController::AGameController():
StartPoint(nullptr),
EndPoint(nullptr),
EnemySpawnPoint(nullptr),
CurrentSelectedActor(nullptr),
GameMode(nullptr),
ARManager(nullptr),
PlayerPawn(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UClass>EndPointBPtemp(TEXT("Class'/Game/Blueprints/endPoint.endPoint_C'"));
	static ConstructorHelpers::FObjectFinder<UClass>StartPointBPtemp(TEXT("Class'/Game/Blueprints/startPoint.startPoint_C'"));	
	static ConstructorHelpers::FObjectFinder<UClass>EnemyBPtemp(TEXT("Class'/Game/Blueprints/EnemySpawnPoint.EnemySpawnPoint_C'"));
	static ConstructorHelpers::FObjectFinder<UClass>BulletBPtemp(TEXT("Class'/Game/Blueprints/BulletBP.BulletBP_C'"));

	if (EndPointBPtemp.Object)
		EndPointBP = EndPointBPtemp.Object;

	if(StartPointBPtemp.Object)
		StartPointBP= StartPointBPtemp.Object;

	if(EnemyBPtemp.Object)
		EnemyBP= EnemyBPtemp.Object;

	if (BulletBPtemp.Object)
		BulletBP = BulletBPtemp.Object;
}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Reloading)
	{
		CurrentReloadTime -= DeltaTime;

		if (CurrentReloadTime <= 0)
		{
			SetScore(10);
			Reloading = false;
			CurrentReloadTime = 0;
		}
	}

	if (GetStartSpawned())
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARPlaneActor::StaticClass(), Planes);
	

	switch (UARBlueprintLibrary::GetARSessionStatus().Status)
	{
	case EARSessionStatus::Running:

		if (GameMode->GetGameState<ACustomGameState>()->GetCustomGamePhase() == creator)
		{
			if (!GetSelect())
				ARManager->UpdatePlaneActors();

			if (GetStartSpawned() && GetEndSpawned())
				for (AActor* it : Planes)
					it->SetActorHiddenInGame(true);
		}
		break;

	case EARSessionStatus::FatalError:

		ARManager->ResetARCoreSession();
		break;
	}
}

UARPin* AGameController::PlayerTouchPin(FVector ScreenPos)
{
	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;

	if (!playerController) return nullptr;

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);
	// Notice that this LineTrace is in the ARBluePrintLibrary - this means that it's exclusive only for objects tracked by ARKit/ARCore
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);
	
	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();
		// Get the first found object in the line trace - ignoring the rest of the array elements

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
			return UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());
	}

	return nullptr;
}

void AGameController::LineTraceSpawnActor(FVector ScreenPos)
{
	if (GameMode->GetGameState<ACustomGameState>()->GetCustomGamePhase() == 0) return;	//if in idle phase this function does nothing 

	if (GameMode->GetGameState<ACustomGameState>()->GetCustomGamePhase() == 2)	//if in player phase
	{
		if (Reloading) return;

		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(PlayerPawn->GetCameraRotation());
		const FVector MyLoc(PlayerPawn->GetCameraLocation());

		Bullet = GetWorld()->SpawnActor<AActor>(BulletBP, MyLoc, MyRot, SpawnInfo);

		int x = GetScore();
		SetScore(x-1);

		if (GetScore() <= 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("no ammo"));
			Reloading = true;
			CurrentReloadTime = ReloadTime;
		}
		return;
	}

	//Spawn the actor pin and get the transform
	UARPin* ActorPin = PlayerTouchPin(ScreenPos);
	// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
	if (ActorPin != nullptr)
	{
		//If the pin is valid 
		auto PinTF = ActorPin->GetLocalToWorldTransform();

		//Spawn start point actor if it does not exist yet
		if (!StartPoint)
		{
			const FActorSpawnParameters SpawnInfo;
			const FRotator MyRot(0, 0, 0);
			const FVector MyLoc(0, 0, 0);
			StartPoint = GetWorld()->SpawnActor<APlaceableActor>(StartPointBP, MyLoc, MyRot, SpawnInfo);

			SetStartSpawned(true);
			StartPoint->SetActorTransform(PinTF);
			StartPoint->PinComponent = ActorPin;
			return;
		}

		//Spawn end point actor if startpoint exists and end point does not
		if (StartPoint && !EndPoint)
		{
			const FActorSpawnParameters SpawnInfo;
			const FRotator MyRot(0, 0, 0);
			const FVector MyLoc(0, 0, 0);
			EndPoint = GetWorld()->SpawnActor<APlaceableActor>(EndPointBP, MyLoc, MyRot, SpawnInfo);

			SetEndSpawned(true);
			EndPoint->SetActorTransform(PinTF);
			EndPoint->PinComponent = ActorPin;
			return;
		}		
		
		if (StartPoint && EndPoint)	
		{
			if (EnemySpawnLocs.Num() <= 10)
			{
				const FActorSpawnParameters SpawnInfo;
				const FRotator MyRot(0, 0, 0);
				const FVector MyLoc(0, 0, 0);
				EnemySpawnPoint = GetWorld()->SpawnActor<APlaceableActor>(EnemyBP, MyLoc, MyRot, SpawnInfo);

				SetEnemySpawned(true);
				EnemySpawnPoint->SetActorTransform(PinTF);
				EnemySpawnPoint->PinComponent = ActorPin;

				//spawn locations are added to this array so they can all be activated at the start of the player phase
				EnemySpawnLocs.Push(EnemySpawnPoint);
			}
		}
	}
}

void AGameController::PlayerTouchHeld(FVector ScreenPos)
{
	if (GameMode->GetGameState<ACustomGameState>()->GetCustomGamePhase() != 1)	//if in idle phase this function does nothing 
		return;

	if (StartPoint || EndPoint)
	{
		bool hit = WorldHitTest(FVector2D(ScreenPos), hitTestResult);
		if (!hit)
			return;

		if (hitTestResult.GetActor()->IsA(APlaceableActor::StaticClass()))
		{
			auto temp = Cast<APlaceableActor>(hitTestResult.GetActor());
			SetSelect(true, temp);
		}
	}

	UARPin* ActorPin = PlayerTouchPin(ScreenPos);

	if (ActorPin != nullptr)
	{
		auto PinTF = ActorPin->GetLocalToWorldTransform();
		// Set the spawned actor location based on the Pin. Have a look at the code for Placeable Object to see how it handles the AR PIN passed on
		if (GetSelect()) {
			CurrentSelectedActor->SetActorTransform(PinTF);
			CurrentSelectedActor->PinComponent = ActorPin;
		}
	}
}

bool AGameController::WorldHitTest(FVector2D screenPos, FHitResult& hitResult)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection; // Unit Vector
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/
		worldPosition, /*out*/ worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = worldDirection * 1000.0;
	traceEndVector = worldPosition + traceEndVector;
	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, worldPosition, traceEndVector,
		ECollisionChannel::ECC_WorldDynamic);
	// return if the operation was successful
	return traceSuccess;
}

void AGameController::ActivateSpawnLocs()
{
	if (EnemySpawnLocs.Num() > 0)
	{
		for (auto it : EnemySpawnLocs)
		{
			auto obj = Cast<APlaceableActor>(it);
			obj->SpawnEnemyPawn();
		}
	}
}

int32 AGameController::GetScore()
{
	return GameMode->GetGameState<ACustomGameState>()->Bullets;
}

void AGameController::SetScore(const int32 NewScore)
{
	GameMode->GetGameState<ACustomGameState>()->Bullets = NewScore;
}

bool AGameController::GetStartSpawned()
{
	return GameMode->GetGameState<ACustomGameState>()->StartSpawned;
}

void AGameController::SetStartSpawned(const bool spawned)
{
	GameMode->GetGameState<ACustomGameState>()->StartSpawned = spawned;
}

bool AGameController::GetEndSpawned()
{
	return GameMode->GetGameState<ACustomGameState>()->EndSpawned;
}

void AGameController::SetEndSpawned(const bool spawned)
{
	GameMode->GetGameState<ACustomGameState>()->EndSpawned = spawned;
}

bool AGameController::GetEnemySpawned()
{
	return GameMode->GetGameState<ACustomGameState>()->EnemySpawned;
}

void AGameController::SetEnemySpawned(const bool spawned)
{
	GameMode->GetGameState<ACustomGameState>()->EnemySpawned = spawned;
}

int AGameController::GetGamePhase()
{
	return GameMode->GetGameState<ACustomGameState>()->GetCustomGamePhase();
}

void AGameController::SetGamePhase(EGamePhase newGameStatePhase)
{
	GameMode->GetGameState<ACustomGameState>()->ECurrentGamePhase = newGameStatePhase;
}

bool AGameController::GetSelect()
{
	return GameMode->GetGameState<ACustomGameState>()->CubeSelected;
}

void AGameController::SetSelect(const bool selected, APlaceableActor* selectedActor)
{
	if (!selectedActor && !selected && CurrentSelectedActor)	//check if spawned actor exists
	{
		CurrentSelectedActor->SetSelect(selected);
	}

	CurrentSelectedActor = selectedActor;
	GameMode->GetGameState<ACustomGameState>()->CubeSelected = selected;

	if(selectedActor)
		selectedActor->SetSelect(selected);
}

void AGameController::SetGameMode(AGameModeBase* newGameMode)
{
	GameMode = newGameMode;
}

void AGameController::SetARmanager(AHelloARManager* newARManager)
{
	ARManager = newARManager;
}