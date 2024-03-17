// Fill out your copyright notice in the Description page of Project Settings.


#include "IconActor.h"

// Sets default values
AIconActor::AIconActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Actor root"));
	SetRootComponent(ActorRoot);

	BombMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb mesh"));
	BombMeshComp->SetupAttachment(ActorRoot);
	BombMeshComp->SetVisibility(false);
	BombMeshComp->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	EmptyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty mesh"));
	EmptyMeshComp->SetupAttachment(ActorRoot);
	EmptyMeshComp->SetVisibility(false);
	EmptyMeshComp->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
}

// Called when the game starts or when spawned
void AIconActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIconActor::ShowIcon(const int32& val)
{
	if (val == 0)
	{
		EmptyMeshComp->SetRelativeRotation(FRotator(0.0f, rand(), rand() % 1));
		EmptyMeshComp->SetVisibility(true);
	}
	if (val < 9 && val > 0)
	{
		ShowNumberIcon(val - 1);
		EmptyMeshComp->SetRelativeRotation(FRotator(0.0f, rand(), rand() % 1));
		EmptyMeshComp->SetVisibility(true);
	}
	if (val >= 9)
	{
		BombMeshComp->SetVisibility(true);
		EmptyMeshComp->SetRelativeRotation(FRotator(0.0f, rand(), rand() % 1));
		EmptyMeshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		EmptyMeshComp->SetVisibility(true);
	}
}

void AIconActor::ShowNumberIcon_Implementation(const int32& val)
{
}

