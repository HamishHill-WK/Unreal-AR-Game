// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;

UCLASS()
class UE5_AR_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);	
	virtual void OnScreenHeld(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);
	virtual void OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);

	float TouchTime = 0;
	float HoldTimeMin = 0.5;

	bool ScreenIsTouched = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = "player_Variables", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "player_Variables", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	UPROPERTY(Category = "player_Variables", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(Category = "player_Variables", VisibleAnywhere, BlueprintReadWrite)
		float Health = 1.f;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI_Functions")
		void UISetCrossHair(bool showCH);	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI_Functions")
		void UISetStartButton(bool showSB);	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI_Functions")
		void UISetExitButton(bool showSB);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI_Functions")
		void PlayerDead();	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CustomAudio")
		void ShootSound();

	UFUNCTION(BlueprintCallable, Category = "UI_Functions")
		void Dead();

	UFUNCTION(BlueprintCallable, Category = "UI_Functions")
		void StartPlayerMode();	
	
	UFUNCTION(BlueprintCallable, Category = "UI_Functions")
		void ShowPlayButton();	
	
	UFUNCTION(BlueprintCallable, Category = "UI_Functions")
		void ShowExitButton();

	UFUNCTION(BlueprintCallable, Category = "player_Functions")
		void ReduceHealth();

	AActor* Temp;

	bool Creator; 

	float ImmuneTimer;
	float ImmuneTime = 5.f;	

	UPROPERTY(Category = "player_Variables", VisibleAnywhere, BlueprintReadWrite)
		bool Immune = false;
};
