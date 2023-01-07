// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomARPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ARBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "CustomGameMode.h"

// Sets default values
ACustomARPawn::ACustomARPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/Meshes/beretta_m9.beretta_m9'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	StaticMeshComponent->SetupAttachment(CameraComponent);
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

	FVector defaultLoc (30, 0, -60);
	FRotator defaultRot(0, 0, 0);
	defaultRot.Yaw = 90;
	StaticMeshComponent->SetRelativeRotation(defaultRot);	
	StaticMeshComponent->SetRelativeLocation(defaultLoc);
	StaticMeshComponent->SetHiddenInGame(true);				//gun mesh is added to the player object and set to hidden 

	Temp = UGameplayStatics::GetActorOfClass(GetWorld(), AGameController::StaticClass());	//variable to hold game controller object
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Immune)		//set to true when the player takes damage. 
	{
		ImmuneTime -= DeltaTime;

		if (ImmuneTime <= 0)
			Immune = false;
	}

	if (ScreenIsTouched)
	{
		TouchTime += DeltaTime;
	}
}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);	
	PlayerInputComponent->BindTouch(IE_Repeat, this, &ACustomARPawn::OnScreenHeld);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACustomARPawn::OnScreenRelease);
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{	
	ScreenIsTouched = true;
	TouchTime = 0;
	auto GM = Cast<AGameController>(Temp);

	if (!GM) return;

	if (GM->GetGamePhase() == player && !GM->Reloading)
		ShootSound();

	GM->LineTraceSpawnActor(ScreenPos);
}

void ACustomARPawn::OnScreenHeld(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	if (TouchTime == 0) 
		return;

	if (TouchTime >= HoldTimeMin)
	{
		auto GM = Cast<AGameController>(Temp);

		if (GM->GetGamePhase() == creator)
		{
			if (!GM) return;

			GM->PlayerTouchHeld(ScreenPos);
		}
	}
}

void ACustomARPawn::OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	ScreenIsTouched = false;
	TouchTime = 0;
	auto GM = Cast<AGameController>(Temp);
	
	if (!GM) return;

	if(GM->GetGamePhase() == creator)
		GM->SetSelect(false, nullptr);
}

void ACustomARPawn::StartPlayerMode()
{
	auto GM = Cast<AGameController>(Temp);

	if (!GM) return;

	this->UISetCrossHair(true);
	StaticMeshComponent->SetHiddenInGame(false);
	this->UISetStartButton(false);
	GM->ActivateSpawnLocs();
}

void ACustomARPawn::ShowPlayButton() 	//display start game button when in creator mode, start + endpoint exist + at least 1 enemy.
{										// Called from startpoint BP when player is inside its capsule
	auto GM = Cast<AGameController>(Temp);

	if (!GM) return;

	if (GM->GetGamePhase() != creator) return;

	if (GM->GetStartSpawned() && GM->GetEndSpawned() && GM->GetEnemySpawned())		
		this->UISetStartButton(true);
}

void ACustomARPawn::ShowExitButton() 	//display exit game button when in player mode, start + endpoint exist + at least 1 enemy. 
{										//Called from endpoint BP when player is inside its capsule 
	auto GM = Cast<AGameController>(Temp);

	if (!GM) return;

	if (GM->GetGamePhase() != 2) return;

	if (GM->GetStartSpawned() && GM->GetEndSpawned() && GM->GetEnemySpawned())
		this->UISetStartButton(true);
}

void ACustomARPawn::ReduceHealth()
{
	if (Immune) return;

	ImmuneTime = ImmuneTimer; 

	Immune = true;
	Health -= 0.1f;
	
	if (Health <= 0.0f)
		Dead();
}

void ACustomARPawn::Dead() 
{
	this->UISetStartButton(false);

	PlayerDead();

	auto GM = Cast<AGameController>(Temp);

	if (!GM) return;

	GM->SetGamePhase(idle);
}