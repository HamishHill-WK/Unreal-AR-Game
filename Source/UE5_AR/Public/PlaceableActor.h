// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomARPawn.h"
#include "GameFramework/Actor.h"
#include "PlaceableActor.generated.h"

class UARPin;

UCLASS()
class UE5_AR_API APlaceableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void setColour(FColor newColor);	
	
	virtual bool GetSelect();
	
	virtual void SetSelect(bool newSelect);

	UFUNCTION(BlueprintImplementableEvent, Category = "myCategory")
		void SpawnEnemyPawn();

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(Category = "myCategory", EditAnywhere)
		UMaterialInstanceDynamic* MMaterial;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		FVector ColourVec;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		APawn* playerPawn;	
	
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		bool Selected;

	UARPin* PinComponent;
};
