// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	StaticMeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent1"));
	StaticMeshComponent1->SetupAttachment(StaticMeshComponent);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Blueprints/Low_poly_UFO_UFO_body.Low_poly_UFO_UFO_body'"));

	if(MeshAsset.Object)
		StaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	auto MeshAsset1 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Blueprints/Low_poly_UFO_UFO_cockpit.Low_poly_UFO_UFO_cockpit'"));

	if (MeshAsset1.Object)
		StaticMeshComponent1->SetStaticMesh(MeshAsset1.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial>DeathMat(TEXT("Material'/Game/Character_Appearance_Dissolve/Materials/Mobile/M_Appearance_11_Mobile.M_Appearance_11_Mobile'"));

	if (DeathMat.Object)
	{
		DeathMaterial = DeathMat.Object;
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = UMaterialInstanceDynamic::Create(DeathMaterial, GetOuter());
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DeathTimer > 0)
	{
		DeathTimer -= DeltaTime;

		if (DeathTimer <= 0)
			Destroy();

		return;
	}

	if(Immune)
	{
		ImmuneTimer -= DeltaTime;

		if (ImmuneTimer <= 0)
			Immune = false;
	}

	if (CanSeePlayer && !Dead)
	{
		if (Retreat)
		{
			RetreatTimer -= DeltaTime;
			FVector movement = FMath::VInterpTo(this->GetActorLocation(), RetreatLoc, DeltaTime, 0.75f);
			SetActorLocation(movement);

			if (RetreatTimer <= 0)
				Retreat = false;
		}

		if (!Retreat)
		{
			FVector movement = FMath::VInterpTo(this->GetActorLocation(), TargetLoc, DeltaTime, 0.33f);
			SetActorLocation(movement);
		}
	}
}

void AEnemy::FlyAtPlayer(FVector newTargetLoc)
{
	CanSeePlayer = true;
	TargetLoc = newTargetLoc;
}

void AEnemy::RetreatFunc(FVector newTargetLoc)
{
	RetreatTimer = RetreatTime;
	Retreat = true;
	RetreatLoc = newTargetLoc;
}


void AEnemy::EnemyDie()
{
	StaticMeshComponent->SetMaterial(0, DynamicMaterial);
	StaticMeshComponent1->SetMaterial(0, DynamicMaterial);
	DeathTimer = DeathTime;
	StaticMeshComponent->SetSimulatePhysics(true);
	Dead = true;
	HpVisible = false;
}

void AEnemy::ReduceHealth()
{
	if (Immune) return;
	
	if (!HpVisible)
		HpVisible = true;

	Health -= 1.f;

	if (Health <= 0)
		EnemyDie();

	if (!Immune)
	{
		ImmuneTimer = ImmuneTime; 
		Immune = true;
	}
}