// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardSegment.h"

// Sets default values
ABoardSegment::ABoardSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoardSegmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(BoardSegmentMesh);
}

// Called when the game starts or when spawned
void ABoardSegment::BeginPlay()
{
	Super::BeginPlay();
	
	RowSize = GameControllerRef->GameCore->GetRow();
	ColumnSize = GameControllerRef->GameCore->GetColumn();
	FVector BoardSize = FVector(RowSize, ColumnSize, 1.0f);
	BoardSegmentMesh->SetRelativeScale3D(BoardSize);
}

// Called every frame
void ABoardSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

