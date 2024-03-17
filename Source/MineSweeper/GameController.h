// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "CursorActor.h"
#include "Engine/World.h"
#include "FlagActor.h"
#include "GameCore/MineSweeperCore.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "IconActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MineSweeper.h"
#include "GameController.generated.h"

UCLASS()
class MINESWEEPER_API AGameController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Control
	TPair<int32, int32> MouseLocation;
	APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Game core
	MineSweeperCore* GameCore = new MineSweeperCore();

	// Game play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board to spawn")
		TSubclassOf<AActor> Board;
	AActor* BoardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor on board")
		TSubclassOf<AActor> CursorOnBoard;
	AActor* CursorOnBoardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon on board")
		TSubclassOf<AActor> IconOnBoard;
	TArray<AActor*> IconOnBoardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag on board")
		TSubclassOf<AActor> FlagOnBoard;
	TArray<TArray<AActor*>> FlagOnBoardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu cursor")
		bool bMenuOpened;

private:
	TArray<AActor*> SummonedActor;
	bool bCursorShow;

public:
	void YawInput(float val);
	void PitchInput(float val);
	void Zoom(float val);
	void MouseLineTracing(FHitResult& HitResult);
	void LeftClickOn();
	void MiddleClickOn();

	UFUNCTION(BlueprintCallable)
		void StartGame(const int32& Setrow, const int32& Setcolumn, const int32& SetBomb);

	UFUNCTION(BlueprintCallable)
		void StepOn(const int32& Onrow, const int32& Oncolumn);

	UFUNCTION(BlueprintCallable)
		void SetFlag(const int32& Onrow, const int32& Oncolumn);

	UFUNCTION(BlueprintCallable)
		void Reveal();

private:
	void HideCursor();
	void ShowCursor();
	void CursorAtCenter();
	void CursorPointing();

	void SetSpawnFlagSize();

	// Spawn actor
	void SpawnBoard();
	void SpawnCursorOnBoard();
	void SpawnIcon(const int32& Onrow, const int32& Oncolumn);
	void ToggleSpawnFlag(const int32& Onrow, const int32& Oncolumn);
	void DestoryActor();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
		USceneComponent* PlayerRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spring arm")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* TopCamera;
	
};
