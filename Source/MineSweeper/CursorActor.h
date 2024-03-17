// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameCore/MineSweeperCore.h"
#include "GameFramework/Actor.h"
#include "CursorActor.generated.h"

class UCurveFloat;

UCLASS()
class MINESWEEPER_API ACursorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACursorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Mine sweeper
	MineSweeperCore* GameCore;

	bool bShowCursor;

	// Moving timeline
	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve graph")
		UCurveFloat* CurveFloat;
	FVector StartLocation;
	FVector EndLocation;

private:
	FVector DefaultSize = FVector(0.3f, 0.3f, 0.3f);

public:
	void MoveToLocation(FVector Location);

	void HideCursor();
	void ShowCursor();

	UFUNCTION()
		void TimelineProgress(float val);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
		USceneComponent* ActorRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
		UStaticMeshComponent* DigHoleHint;

};
