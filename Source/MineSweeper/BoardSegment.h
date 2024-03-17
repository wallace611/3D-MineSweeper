// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameController.h"
#include "GameFramework/Actor.h"
#include "BoardSegment.generated.h"

UCLASS()
class MINESWEEPER_API ABoardSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardSegment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* BoardSegmentMesh;

private:
	AGameController* GameControllerRef = Cast<AGameController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	int32 RowSize;
	int32 ColumnSize;

};
