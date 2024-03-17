// Fill out your copyright notice in the Description page of Project Settings.


#include "MineSweeperCore.h"

void MineBoard::ResetBoard()
{
	Board.Empty();
	Interface.Empty();
	Called.Empty();
}

// return value:
// values <= 0: -1
// set size successfully: 0
int32 MineSweeperCore::SetSize(const int32& Setrow, const int32& Setcolumn)
{
	if (Setrow <= 0 || Setcolumn <= 0)
		return -1;
	row = Setrow;
	column = Setcolumn;
	size = row * column;
	return 0;
}

int32 MineSweeperCore::SetBomb(const int32& SetBomb)
{
	if (SetBomb <= 0)
		return -1;
	bombs = SetBomb;
	BombQuantity = bombs;
	return 0;
}

// return values:
// row or column <= 0: -1
// generated successfully: 0
int32 MineSweeperCore::Generate()
{
	if (row <= 0 || column <= 0)
		return -1;
	// Generating the board
	for (int i = 0; i < row; i++)
	{
		TArray<int32> temp;
		TArray<char> Itemp;
		TArray<bool> Btemp;
		for (int j = 0; j < column; j++) Itemp.Push('.');
		temp.SetNum(column);
		Btemp.SetNum(column);
		Board.Board.Push(temp);
		Board.Interface.Push(Itemp);
		Board.Called.Push(Btemp);
	}
	Generated = true;
	return 0;
}

void MineSweeperCore::Reset()
{
	Board.ResetBoard();
	GameEnd = 0;
	BombQuantity = 0;
	LastSet = { -1, -1 };
	row = 0;
	column = 0;
	NoBombArea = 0;
	BombLocation.Empty();
	Generated = 0;
}

// get the value of row and column
int32 MineSweeperCore::GetRow()
{
	return row;
}
int32 MineSweeperCore::GetColumn()
{
	return column;
}

int32 MineSweeperCore::GetSize()
{
	return row * column;
}

// return values:
// board hasn't generated: -10
// reveal successfully: 0
int32 MineSweeperCore::Reveal()
{
	if (!Generated)
		return -10;
	for (int i = 0; i < BombLocation.Num(); i++)
	{
		Board.Interface[BombLocation[i].Key][BombLocation[i].Value] = '*';
	}
	return 0;
}

// return values:
// board hasn't generated: -10
// number is not in range: -3
// step on a flag: -2
// had already been stepped: -1
// normal consequence: 0
// step on a bomb: 1
// all bombs are cleared: 2
int32 MineSweeperCore::StepOn(const int32& Onrow, const int32& Oncolumn, TArray<TPair<int32, int32>>& SteppedArea)
{
	// Check
	if (!Generated)
		return -10;
	if (!InRange(Onrow, Oncolumn))
		return -3;

	// Generate Bombs
	if (LastSet.Key == -1 && LastSet.Value == -1)
	{
		GenerateBomb(Onrow, Oncolumn);
	}

	LastSet = { Onrow, Oncolumn };
	if (Board.Interface[Onrow][Oncolumn] != '.')
		return -1;
	if (Board.Interface[Onrow][Oncolumn] == 'F')
		return -2;

	// Step on
	LastSet = TPair<int32, int32>(Onrow, Oncolumn);
	if (Board.Board[Onrow][Oncolumn] > 8)
	{
		SteppedArea.Push(LastSet);
		Board.Interface[Onrow][Oncolumn] = '*';
		GameEnd = 1;
		return 1;
	}
	FindArea(Onrow, Oncolumn, SteppedArea);
	if (NoBombArea == 0)
	{
		GameEnd = 2;
		return 2;
	}
	return 0;
}

// return values:
// board hasn't generated: -10
// number is not in range: -3
// failed to put a flag: -1
// put the flag successfully: 0
int32 MineSweeperCore::ToggleFlag(const int32& Onrow, const int32& Oncolumn)
{
	if (!Generated)
		return -10;
	if (!InRange(Onrow, Oncolumn))
		return -3;
	if (Board.Interface[Onrow][Oncolumn] == '.')
	{
		Board.Interface[Onrow][Oncolumn] = 'F';
		BombQuantity--;
		return 0;
	}
	if (Board.Interface[Onrow][Oncolumn] == 'F')
	{
		Board.Interface[Onrow][Oncolumn] = '.';
		BombQuantity++;
		return 0;
	}
	return -1;
}

int32 MineSweeperCore::Random()
{
	return rand();
}

bool MineSweeperCore::InRange(const int32& Onrow, const int32& Oncolumn)
{
	if (Onrow < row && Onrow >= 0 && Oncolumn < column && Oncolumn >= 0)
		return 1;
	return 0;
}

int32 MineSweeperCore::SetAvoidLocation(const int32& Avoidrow, const int32& Avoidcolumn, TArray<int32>& NoBomb)
{
	for (int i = Avoidrow - 1; i <= Avoidrow + 1; i++)
	{
		for (int j = Avoidcolumn - 1; j <= Avoidcolumn + 1; j++)
		{
			if (i >= 0 && i < row && j >= 0 && j < column)
			{
				int32 temp = i * column + j;
				NoBomb.RemoveAt(NoBomb.Find(temp));
			}
		}
	}
	return 0;
}

int32 MineSweeperCore::GenerateBomb(const int32& Avoidrow, const int32& Avoidcolumn)
{
	TArray<int32> NoBomb;
	for (int i = 0; i < size; i++)
		NoBomb.Push(i);
	SetAvoidLocation(Avoidrow, Avoidcolumn, NoBomb);

	// Set bombs
	for (int i = 0; i < bombs; i++)
	{
		int Location = Random() % NoBomb.Num();
		int temp = NoBomb[Location];
		NoBomb.RemoveAt(Location);

		const int32 Bombrow = temp / column;
		const int32 Bombcolumn = temp % column;
		BombLocation.Push(TPair<int32, int32>(Bombrow, Bombcolumn));
		Board.Board[Bombrow][Bombcolumn] = 9;
	}

	// Setup NoBombArea
	NoBombArea = row * column - bombs;

	// Generating the numbers
	for (int i = 0; i < BombLocation.Num(); i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			for (int k = -1; k <= 1; k++)
			{
				if (BombLocation[i].Key + j >= 0 && BombLocation[i].Key + j < row && BombLocation[i].Value + k >= 0 && BombLocation[i].Value + k < column)
				{
					Board.Board[BombLocation[i].Key + j][BombLocation[i].Value + k]++;
				}
			}
		}
	}
	return 0;
}

void MineSweeperCore::FindArea(int32 Onrow, int32 Oncolumn, TArray<TPair<int32, int32>>& SteppedArea)
{
	TQueue<TPair<int32, int32>> NextVisit;
	NextVisit.Enqueue(TPair<int32, int32>(Onrow, Oncolumn));
	while (!NextVisit.IsEmpty())
	{
		Onrow = NextVisit.Peek()->Key;
		Oncolumn = NextVisit.Peek()->Value;
		if (Board.Called[Onrow][Oncolumn])
		{
			NextVisit.Pop();
			continue;
		}
		SteppedArea.Push(*NextVisit.Peek());
		if (Board.Board[Onrow][Oncolumn] == 0)
		{
			if (Onrow - 1 >= 0)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow - 1, Oncolumn));
			if (Onrow + 1 < row)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow + 1, Oncolumn));
			if (Oncolumn - 1 >= 0)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow, Oncolumn - 1));
			if (Oncolumn + 1 < column)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow, Oncolumn + 1));
			if (Onrow - 1 >= 0 && Oncolumn - 1 >= 0)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow - 1, Oncolumn - 1));
			if (Onrow + 1 < row && Oncolumn - 1 >= 0)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow + 1, Oncolumn - 1));
			if (Onrow - 1 >= 0 && Oncolumn + 1 < column)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow - 1, Oncolumn + 1));
			if (Onrow + 1 < row && Oncolumn + 1 < column)
				NextVisit.Enqueue(TPair<int32, int32>(Onrow + 1, Oncolumn + 1));
		}
		if (Board.Interface[Onrow][Oncolumn] == 'F')
			BombQuantity++;
		Board.Interface[Onrow][Oncolumn] = Board.Board[Onrow][Oncolumn] + 48;
		Board.Called[Onrow][Oncolumn] = 1;
		NoBombArea--;
		NextVisit.Pop();
	}
}

void MineSweeperActions::PrintBoard(MineSweeperCore* GameCore)
{
	FString ColumnNumber = TEXT("  ");
	UE_LOG(LogTemp, Log, TEXT(" "));
	for (int i = GameCore->GetRow() - 1; i >= 0; i--)
	{
		FString s;
		s.AppendChar(i % 10 + 48);
		s.AppendChar(' ');
		FString Itemp;
		for (int j = 0; j < GameCore->GetColumn(); j++)
		{
			if (TPair<int32, int32>(i, j) == GameCore->LastSet)
				s.AppendChar('S');
			else
				s.AppendChar(GameCore->Board.Interface[i][j]);
			if (GameCore->Board.Board[i][j] < 9)
				Itemp.AppendChar(GameCore->Board.Board[i][j] + 48);
			else
				Itemp.AppendChar('*');
		}
		s.Append(TEXT(" "));
		s.AppendChar(i % 10 + 48);
		s.AppendChar(' ');
		s.Append(Itemp);
		UE_LOG(LogTemp, Log, TEXT("%s"), *s);
	}
	UE_LOG(LogTemp, Log, TEXT(" "));
	for (int i = 0; i < GameCore->GetColumn(); i++)
	{
		ColumnNumber.AppendChar(i % 10 + 48);
	}
	UE_LOG(LogTemp, Log, TEXT("%s %s"), *ColumnNumber, *ColumnNumber);
	UE_LOG(LogTemp, Log, TEXT(" "));
	UE_LOG(LogTemp, Log, TEXT("There are %d bombs"), GameCore->BombQuantity);
	UE_LOG(LogTemp, Log, TEXT(" "));
}

