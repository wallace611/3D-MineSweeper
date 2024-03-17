#include "MineSweeperCore.h"
#include <stdlib.h>
using namespace std;

class MineInShell
{
    private:
        void PrintBoard();
    public:
        MineSweeper GameCore;
        bool Quit = 0;
    public:
        void Input();
        void GameStart();
        void PrintHelp();
};

MineInShell GameController;

void MineInShell::Input()
{
    int row, column;
    char command;
    cin >> command;
    
    switch (command)
    {
        case 'i':
            cin >> row >> column;
            if(GameCore.StepOn(row, column) == 1)
            {
                GameCore.GameEnd = -1;
            }
            break;

        case 'f':
            cin >> row >> column;
            GameCore.ToggleFlag(row, column);
            break;

        case 'h':
            PrintHelp();
            break;
        
        case 'r':
            GameCore.Reset();
            break;

        case 'q':
            Quit = 1;
            break;

        default:
            break;
    }
    return;
}

void MineInShell::PrintBoard()
{
    system("CLS");
    printf("     ");
    for(int i = 0; i < GameCore.row; i++)
    {
        printf("%2d ", i + 1);
    }
    printf("  row\n    -");
    for(int i = 0; i < GameCore.row; i++)
        printf("---");
    printf("\n");
    for(int i = 0; i < GameCore.column; i++)
    {
        printf("%4d|", i + 1);
        for(int j = 0; j < GameCore.row; j++)
        {
            if(i == GameCore.LastSet.second && j == GameCore.LastSet.first)
            {
                printf("[%c]", GameCore.Board.Interface[i][j] == '0' ? 32 : GameCore.Board.Interface[i][j]);
                continue;
            }
            printf("%2c ", GameCore.Board.Interface[i][j] == '0' ? 32 : GameCore.Board.Interface[i][j]);
        }
        printf("\n");
    }
    printf("\ncolumn          there are %d bomb(s)\n----------------------------\n", GameCore.BombQuantity);
    return;
}

void MineInShell::PrintHelp()
{
    system("CLS");
    printf("Commands:\n");
    printf(" i [row] [column] to set the next step\n");
    printf(" f [row] [column] to set and unset the flag\n");
    printf(" r to reset\n");
    printf(" q to quit\n\n");
    char a;
    getchar();
    printf("Input any key to go back\n");
    scanf("%c", &a);
    return;
}

void MineInShell::GameStart()
{
    while(!GameCore.GameEnd && !Quit)
    {
        PrintBoard();
        cout << "|| please input the command\n" << (char) 92 << "/ (Input [h] to get some help)" << endl;
        Input();
        if(GameCore.GameEnd == 1)
        {
            system("CLS");
            printf("Are you sure? y/n\n");
            char a;
            getchar();
            scanf("%c", &a);
            if(a == 'y')
            {
                GameCore.GameEnd = 0;
                return;
            }
            else
            {
                GameCore.GameEnd = 0;
                system("CLS");
            }
        }
        if(Quit)
        {
            char a;
            getchar();
            system("CLS");
            printf("Quit? y/n\n");
            scanf("%c", &a);
            if(a == 'n')
                Quit = 0;
        }
    }
    system("CLS");
    GameCore.Reveal();
    PrintBoard();
    if(GameCore.GameEnd == -1)
    {
        printf("BRUH you lose\n");
    }
    if(GameCore.GameEnd == 2)
    {
        printf("You win\n");
    }
    char a;
    getchar();
    printf("Quit? y/n\n");
    scanf("%c", &a);
    if(a == 'y')
        Quit = 1;
    GameCore.Reset();
    GameCore.GameEnd = 0;
}

int main()
{
    unsigned int row, column;
    printf("Input row and column:\n");
    while(scanf("%d%d", &row, &column) != EOF)
    {
        GameController.GameCore.SetSize(row, column);
        GameController.GameCore.Generate();
        GameController.GameStart();
        system("CLS");
        if(GameController.Quit)
            break;
        printf("Input row and column:\n");
    }
    return 0;
}