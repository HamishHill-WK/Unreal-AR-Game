// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HelloARManager.h"
#include "CustomGameState.h"
#include "ARPlaneActor.h"
#include "Camera/PlayerCameraManager.h"
#include "ARPin.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlaceableActor.h"
#include "GameController.generated.h"

UCLASS()
class UE5_AR_API AGameController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameController();

private:
	TArray<AActor*> Planes;

	TArray<AActor*> EnemySpawnLocs;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlaceableActor* StartPoint;
	APlaceableActor* EndPoint;
	APlaceableActor* EnemySpawnPoint;
	AActor* Bullet;

	APlaceableActor* CurrentSelectedActor;

	AGameModeBase* GameMode;

	AHelloARManager* ARManager;

	APlayerCameraManager* PlayerPawn;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LineTraceSpawnActor(FVector ScreenPos);	
	
	virtual UARPin* PlayerTouchPin(FVector ScreenPos);	
	
	virtual void PlayerTouchHeld(FVector ScreenPos);

	UFUNCTION(BlueprintCallable, Category = "selecting")
		bool WorldHitTest(FVector2D screenPos, FHitResult& hitResult);

	UFUNCTION(Category = "GamePhase", BlueprintCallable)
		void SetGamePhase(EGamePhase newGameStatePhase);

	UFUNCTION(Category = "GamePhase", BlueprintCallable)
		int GetGamePhase();

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		FHitResult hitTestResult;

	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> StartPointBP;

	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> EndPointBP;	
	
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> EnemyBP;	
	
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> BulletBP;

	UBlueprint* Spawnable;

	UFUNCTION(BlueprintCallable, Category = "conditions")
		int32 GetScore();

	UFUNCTION(BlueprintCallable, Category = "conditions")
		bool GetStartSpawned();

	UFUNCTION(BlueprintCallable, Category = "conditions")
		bool GetEndSpawned();

	UFUNCTION(BlueprintCallable, Category = "conditions")
		bool GetEnemySpawned();

	UFUNCTION(BlueprintCallable, Category = "conditions")
		bool GetSelect();

	UPROPERTY(Category = "Gun Properties", EditAnywhere, BlueprintReadWrite)
		bool Reloading = false;

	UPROPERTY(Category = "Gun Properties", EditAnywhere, BlueprintReadWrite)
		float ReloadTime = 2.f;

	UPROPERTY(Category = "Gun Properties", EditAnywhere, BlueprintReadWrite)
		float CurrentReloadTime = 0.0f;

	void SetScore(const int32 NewScore);

	void SetStartSpawned(const bool spawned);
	void SetEndSpawned(const bool spawned);
	void SetEnemySpawned(const bool spawned);

	void SetSelect(const bool selected, APlaceableActor* selectedActor);

	void SetGameMode(AGameModeBase* newGameMode);

	void SetARmanager(AHelloARManager* newARManager);

	void ActivateSpawnLocs();
};
