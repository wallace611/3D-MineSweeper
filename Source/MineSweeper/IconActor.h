// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IconActor.generated.h"

UCLASS()
class MINESWEEPER_API AIconActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIconActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void ShowIcon(const int32& val);

	UFUNCTION(BlueprintNativeEvent)
		void ShowNumberIcon(const int32& val);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
		USceneComponent* ActorRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb mesh")
		UStaticMeshComponent* BombMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empty")
		UStaticMeshComponent* EmptyMeshComp;

};
