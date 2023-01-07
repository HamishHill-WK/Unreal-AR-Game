// Fill out your copyright notice in the Description page of Project Settings.
#include "PlaceableActor.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APlaceableActor::APlaceableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
}

// Called when the game starts or when spawned
void APlaceableActor::BeginPlay()
{
	Super::BeginPlay();

	auto Material1 = StaticMeshComponent->GetMaterial(0);
	MMaterial = UMaterialInstanceDynamic::Create(Material1, NULL);

	MMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Blue);

	StaticMeshComponent->SetMaterial(0, MMaterial);

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

bool APlaceableActor::GetSelect()
{
	return Selected;
}

void APlaceableActor::SetSelect(bool newSelect)
{
	Selected = newSelect;
}

// Called every frame
void APlaceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector playerLoc(0,0,0);
	FVector thisLoc(0,0,0);
	FVector diff(0,0,0);
	double dist(0);

	playerLoc = playerPawn->GetActorLocation();
	thisLoc = GetActorLocation();
	diff = playerLoc - thisLoc;
	dist = diff.Length();

	// Making sure the actor remains on the ARPin that has been found.
	if(PinComponent)
	{
		auto TrackingState = PinComponent->GetTrackingState();
		
		switch (TrackingState)
		{
		case EARTrackingState::Tracking:
			SceneComponent->SetVisibility(true);
			SetActorTransform(PinComponent->GetLocalToWorldTransform());
			// Scale down default cube mesh - Change this for your applications.
			SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
			break;

		case EARTrackingState::NotTracking:
			PinComponent = nullptr;
			break;
		}
	}
}

void APlaceableActor::setColour(FColor newColor)
{
	MMaterial->SetVectorParameterValue(TEXT("Color"), newColor);
	StaticMeshComponent->SetMaterial(0, MMaterial);
}

