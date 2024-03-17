// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LevelScriptActor.h"

void ACPP_LevelScriptActor::BeginPlay()
{
	PlayerPawnRef = Cast<AGameController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawnRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to game core successfully"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Cast to game core failed"));
}

void ACPP_LevelScriptActor::MS_StartGame(int32 Setrow, int32 Setcolumn, int32 SetBomb)
{
	this->PlayerPawnRef->StartGame(Setrow, Setcolumn, SetBomb);
}

void ACPP_LevelScriptActor::MS_StepOn(int32 Onrow, int32 Oncolumn)
{
	this->PlayerPawnRef->StepOn(Onrow, Oncolumn);
}

void ACPP_LevelScriptActor::MS_SetFlag(int32 Onrow, int32 Oncolumn)
{
	this->PlayerPawnRef->SetFlag(Onrow, Oncolumn);
}