// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagActor.generated.h"

UCLASS()
class MINESWEEPER_API AFlagActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlagActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Root")
		USceneComponent* ActorRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pillar")
		UStaticMeshComponent* FlagPillar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
		USkeletalMeshComponent* Flag;

};
