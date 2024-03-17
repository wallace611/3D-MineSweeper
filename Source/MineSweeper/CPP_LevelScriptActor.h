// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BoardSegment.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameController.h"
#include "GameCore/MineSweeperCore.h"
#include "CPP_LevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API ACPP_LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AGameController* PlayerPawnRef;

public:
	UFUNCTION(BlueprintCallable)
		void MS_StartGame(int32 Setrow, int32 Setcolumn, int32 SetBomb);
	
	UFUNCTION(BlueprintCallable)
		void MS_StepOn(int32 Onrow, int32 Oncolumn);

	UFUNCTION(BlueprintCallable)
		void MS_SetFlag(int32 Onrow, int32 Oncolumn);
};
