// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableActor.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AEnemy : public APawn
{
	GENERATED_BODY()

	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "enemy_Functions")
		void FlyAtPlayer(FVector newTargetLoc);	
	
	UFUNCTION(BlueprintCallable, Category = "enemy_Functions")
		void RetreatFunc(FVector newTargetLoc);

	UFUNCTION(BlueprintCallable, Category = "enemy_Functions")
		void EnemyDie();

	UFUNCTION(BlueprintCallable, Category = "enemy_Functions")
		void ReduceHealth();	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "enemy_Functions")
		void ShowHealthBar();

	bool CanSeePlayer;

	FVector TargetLoc;
	FVector RetreatLoc;
	
	UPROPERTY(Category = "enemy_Variables", EditAnywhere, BlueprintReadWrite)
		float Health = 4;

	UPROPERTY(Category = "enemy_Variables", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "enemy_Variables", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(Category = "enemy_Variables", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMeshComponent1;

	UPROPERTY(Category = "enemy_Variables", EditAnywhere, BlueprintReadWrite)
		UMaterial* DeathMaterial;

	UPROPERTY(Category = "enemy_Variables", VisibleAnywhere)
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(Category = "enemy_Variables", VisibleAnywhere)
		UMaterialInstanceDynamic* DynamicMaterial1;

	float DeathTimer;
	float DeathTime = 2.f;

	float ImmuneTimer;
	float ImmuneTime = 0.75f;
	bool Immune;

	float RetreatTimer;
	float RetreatTime = 5.f;

	bool HpVisible = false;

	bool Retreat;

	bool Dead = false;
};