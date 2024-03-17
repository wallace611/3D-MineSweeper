// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"
using namespace std;

class MineSweeperCore;

class MineBoard
{
public:
    // shows where the bombs are and the number around it
    TArray<TArray<int32>> Board;

    // the user interface
    TArray<TArray<char>> Interface;

private:
    // record if the segment is visited or not
    TArray<TArray<bool>> Called;

private:
    void ResetBoard();

    friend class MineSweeperCore;
};

class MINESWEEPER_API MineSweeperCore
{
public:
    MineBoard Board;

    // value:
    // normal: 0
    // stepped on a bomb: 1
    // win: 2
    int32 GameEnd;

    // display in the game
    int32 BombQuantity;
    TPair<int32, int32> LastSet = { -1, -1 };
    TArray<TPair<int32, int32>> BombLocation;
    
private:
    int32 row;
    int32 column;
    int32 size;
    int32 bombs;

    // game ends when reachs 0
    int NoBombArea;

    // whether the board has been generated
    // hasn't generated: 0
    // generated: 1
    bool Generated;

public:
    // return value:
    // the board hasn't generated: -10
    // values <= 0: -1
    // set size successfully: 0
    int32 SetSize(const int32& Setrow, const int32& Setcolumn);

    int32 SetBomb(const int32& SetBomb);

    // return values:
    // generated successfully: 0
    int32 Generate();
    
    void Reset();

    // get the value of row and column
    int32 GetRow();
    int32 GetColumn();
    int32 GetSize();

    // return values:
    // board hasn't generated: -10
    // reveal successfully: 0
    int32 Reveal();

    // return values:
    // the board hasn't generated: -10
    // number is not in range: -3
    // step on a flag: -2
    // had already been stepped: -1
    // normal consequence: 0
    // step on a bomb: 1
    // all bombs are cleared: 2
    int32 StepOn(const int32& Onrow, const int32& Oncolumn, TArray<TPair<int32, int32>>& SteppedArea);

    // return values:
    // the board hasn't generated: -10
    // number is not in range: -3
    // failed to put a flag: -1
    // put the flag successfully: 0
    int32 ToggleFlag(const int32& Onrow, const int32& Oncolumn);
    
private:
    int32 Random();

    // verifing if the row and column is (>= 0) and (< row or column)
    bool InRange(const int32& Onrow, const int32& Oncolumn);

    int32 SetAvoidLocation(const int32& Avoidrow, const int32& Avoidcolumn, TArray<int32>& NoBomb);

    int32 GenerateBomb(const int32& Avoidrow, const int32& Avoidcolumn);

    // BFS
    void FindArea(int32 Onrow, int32 Oncolumn, TArray<TPair<int32, int32>>& SteppedArea);
};

class MINESWEEPER_API MineSweeperActions
{
public:
    static void PrintBoard(MineSweeperCore* GameCore);
};