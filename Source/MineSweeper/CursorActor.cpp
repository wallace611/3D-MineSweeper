// Fill out your copyright notice in the Description page of Project Settings.


#include "CursorActor.h"

// Sets default values
ACursorActor::ACursorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ActorRoot"));
	SetRootComponent(ActorRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(ActorRoot);

	DigHoleHint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hint"));
	DigHoleHint->SetupAttachment(ActorRoot);
}

// Called when the game starts or when spawned
void ACursorActor::BeginPlay()
{
	Super::BeginPlay();
	
	HideCursor();
	bShowCursor = 0;
	Mesh->SetRelativeScale3D(DefaultSize);
}

void ACursorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);
}

void ACursorActor::MoveToLocation(FVector Location)
{
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		StartLocation = GetActorLocation();
		EndLocation = Location;
		CurveTimeline.PlayFromStart();
	}
}

void ACursorActor::HideCursor()
{
	Mesh->SetVisibility(false);
	bShowCursor = 0;
}

void ACursorActor::ShowCursor()
{
	Mesh->SetVisibility(true);
	bShowCursor = 1;
}

void ACursorActor::TimelineProgress(float val)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, val);
	SetActorLocation(NewLocation);
}


