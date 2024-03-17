// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"

// Sets default values
AFlagActor::AFlagActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Actor root"));
	SetRootComponent(ActorRoot);

	FlagPillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar"));
	FlagPillar->SetupAttachment(ActorRoot);

	Flag = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Flag"));
	Flag->SetupAttachment(FlagPillar);
}

// Called when the game starts or when spawned
void AFlagActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlagActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

