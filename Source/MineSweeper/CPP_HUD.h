// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameController.h"
#include "CoreMinimal.h"
#include "CPP_HUD.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API UCPP_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		int32 GetBombQuantity();

	UFUNCTION(BlueprintCallable)
		void StartGame(int32 Setrow, int32 Setcolumn, int32 SetBomb);

private:
	AGameController* GameControllerRef = Cast<AGameController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
};
