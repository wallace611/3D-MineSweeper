// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_HUD.h"

int32 UCPP_HUD::GetBombQuantity()
{
	return GameControllerRef->GameCore->BombQuantity;
}

void UCPP_HUD::StartGame(int32 Setrow, int32 Setcolumn, int32 SetBomb)
{
	GameControllerRef->StartGame(Setrow, Setcolumn, SetBomb);
	return;
}
