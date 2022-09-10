#include <iostream>
#include <random>
#include <time.h>

using namespace std;

int board[30][30];

void boardInit();

int main()
{
}

void boardInit()
{
    for (int i = 0; i < 30; ++i)
        for (int j = 0; j < 30; ++j)
        {
            board[i][j] = 0;
        }
}