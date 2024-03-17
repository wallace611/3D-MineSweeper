#include <iostream>
#include <vector>
#include <utility>
#include <map>
using namespace std;

class MineSweeper;

class MineBoard
{
    public:
        vector<vector<char>> Interface;
        vector<vector<int>> Board;
        vector<vector<bool>> Called;
};

class MineSweeper
{
    public:
        unsigned int row;
        unsigned int column;
        MineBoard Board;
        int GameEnd = 0;
        int BombQuantity = 0;
        pair<int, int> LastSet = {-1, -1};
    private:
        int NoBombArea = 0;
        vector<pair<int, int>> BombLocation;
    public:
        void SetSize(int Row, int Column);
        void Generate();
        void Reset();
        int StepOn(int Onrow, int Oncolumn);
        int ToggleFlag(int Onrow, int Oncolumn);
        void Reveal();
    private:
        void FindArea(int Onrow, int Oncolumn);
        bool RandomBomb();
};

bool MineSweeper::RandomBomb()
{
    return rand() % 8 == 0 ? 1 : 0;
}

void MineSweeper::FindArea(int Onrow, int Oncolumn)
{
    if(Oncolumn < 0 || Oncolumn > column - 1 || Onrow < 0 || Onrow > row - 1)
        return;
    if(Board.Called[Oncolumn][Onrow])
        return;
    if(Board.Interface[Oncolumn][Onrow] == '0')
        return;
    if(Board.Board[Oncolumn][Onrow] != 0)
    {
        Board.Interface[Oncolumn][Onrow] = Board.Board[Oncolumn][Onrow] + 48;
        Board.Called[Oncolumn][Onrow] = 1;
        NoBombArea--;
        return;
    }
    Board.Interface[Oncolumn][Onrow] = Board.Board[Oncolumn][Onrow] + 48;
    Board.Called[Oncolumn][Onrow] = 1;
    NoBombArea--;
    FindArea(Onrow + 1, Oncolumn);
    FindArea(Onrow, Oncolumn + 1);
    FindArea(Onrow - 1, Oncolumn);
    FindArea(Onrow, Oncolumn - 1);
    return;
}

void MineSweeper::SetSize(int Row, int Column)
{
    row = Row;
    column = Column;
    return;
}

void MineSweeper::Generate()
{
    for(int i = 0; i < column; i++)
    {
        vector<int> temp;
        vector<char> Itemp;
        vector<bool> Btemp;
        for(int j = 0; j < row; j++)
        {
            bool IsBomb = RandomBomb();
            temp.push_back(IsBomb ? 9 : 0);
            if(IsBomb)
                BombLocation.push_back({i, j});
            else
            {
                NoBombArea += 1;
            }
            Itemp.push_back('.');
            Btemp.push_back(0);
        }
        Board.Board.push_back(temp);
        Board.Interface.push_back(Itemp);
        Board.Called.push_back(Btemp);
    }
    BombQuantity = BombLocation.size();
    for(int i = 0; i < BombLocation.size(); i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            for(int k = -1; k <= 1; k++)
            {
                if(BombLocation[i].first + j >= 0 &&BombLocation[i].first + j < column && BombLocation[i].second + k >= 0 &&BombLocation[i].second + k < row)
                {
                    Board.Board[BombLocation[i].first + j][BombLocation[i].second + k]++;
                }
            }
        }
    }
    return;
}

void MineSweeper::Reset()
{
    Board.Board.clear();
    Board.Interface.clear();
    Board.Called.clear();
    GameEnd = 1;
    BombQuantity = 0;
    NoBombArea = 0;
    BombLocation.clear();
    LastSet = {-1, -1};
    return;
}

/* return values:
had already been stepped: -1
normal consequence: 0
step on a bomb: 1
clear all bombs: 2 */
int MineSweeper::StepOn(int Onrow, int Oncolumn)
{
    Onrow--;
    Oncolumn--;
    LastSet = {Onrow, Oncolumn};
    if(Board.Interface[Oncolumn][Onrow] != '.' && Board.Interface[Oncolumn][Onrow] != 'F')
        return -1;
    if(Board.Board[Oncolumn][Onrow] > 8)
    {
        Board.Interface[Oncolumn][Onrow] = '*';
        return 1;
    }
    FindArea(Onrow, Oncolumn);
    if(NoBombArea == 0)
    {
        GameEnd = 2;
        return 2;
    }
    return 0;
}

/* return values:
can't put the flag: -1
put the flag successfully: 0 */
int MineSweeper::ToggleFlag(int Onrow, int Oncolumn)
{
    Onrow--;
    Oncolumn--;
    if(Board.Interface[Oncolumn][Onrow] == '.' || Board.Interface[Oncolumn][Onrow] == 'F')
    {
        Board.Interface[Oncolumn][Onrow] = Board.Interface[Oncolumn][Onrow] == 'F' ? '.' : 'F';
        return 0;
    }
    return -1;
}

void MineSweeper::Reveal()
{
    for(int i = 0; i < BombLocation.size(); i++)
    {
        Board.Interface[BombLocation[i].first][BombLocation[i].second] = '*';
    }
    return;
}