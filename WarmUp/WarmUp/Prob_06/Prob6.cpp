#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <random>
#include <time.h>

#define LEFT 0b0001
#define RIGHT 0b0010
#define UP 0b0100
#define DOWN 0b1000

using namespace std;

int board[30][30];

void boardPrint();
void boardInit();
void boardMakeRoute();

int main()
{
    boardInit();
    boardMakeRoute();
    boardPrint();

    char commend = -1;

    while (commend != 'q')
    {
        cout << "Commend : ";
        cin >> commend;
        switch (commend)
        {
        case 'r':
            system("cls");
            boardInit();
            boardMakeRoute();
            boardPrint();
            break;
        case 'a': // move route left
            system("cls");
            boardPrint();
            break;
        case 'd': // move route right
            system("cls");
            boardPrint();
            break;
        }
    }
}

void boardPrint()
{
    for (int i = 0; i < 30; ++i)
    {
        for (int j = 0; j < 30; ++j)
        {
            printf("%2d ", board[i][j]);
        }
        cout << "\n";
    }
}

void boardInit()
{
    for (int i = 0; i < 30; ++i)
        for (int j = 0; j < 30; ++j)
        {
            board[i][j] = 0;
        }
}

void boardMakeRoute()
{
    // 조건 1 : 한 방향으로 최대 8칸 이동
    // 조건 2 : 경로를 좌우상하로 최소 한 번 이동
    // 조건 3 : 한 번 지나갔던 길을 지나면 가장 마지막의 값으로 출력

    srand(time(NULL));

    int count = 0;
    int xPos = 0;
    int yPos = 0;
    int direction = RIGHT;

    int leftCnt = 0;
    int upCnt = 0;

    int conditionCount = 0;
    int conditionDirection = 0;

    bool isSafe;
    bool isSameDirection;

    while (1)
    {
        if (conditionCount == 7)
            conditionCount = 0;

        board[xPos][yPos] = ++count;

        if (xPos == 29 && yPos == 29 && conditionDirection == 0b1111)
            break;

        ++conditionCount;

        do
        {
            isSafe = true;
            switch (direction)
            {
            case RIGHT:
                if (yPos >= 29)
                {
                    isSafe = false;
                }
                else
                {
                    ++yPos;
                    conditionDirection |= RIGHT;
                }
                break;

            case LEFT:
                if (yPos <= 0)
                    isSafe = false;
                else
                {
                    --yPos;
                    conditionDirection |= LEFT;
                }
                break;

            case UP:
                if (xPos <= 0)
                    isSafe = false;
                else
                {
                    --xPos;
                    conditionDirection |= UP;
                }
                break;

            case DOWN:
                if (xPos >= 29)
                {
                    isSafe = false;
                }
                else
                {
                    ++xPos;
                    conditionDirection |= DOWN;
                }
                break;
            }

            if (rand() % 5 == 0 || conditionCount == 7 || !isSafe)
            {
                switch (direction)
                {
                case RIGHT:
                    if (!isSafe)
                    {
                        if (board[xPos][yPos - 1] == 0)
                            direction = LEFT;
                        else if (board[xPos + 1][yPos] == 0)
                            direction = DOWN;
                        else if (board[xPos - 1][yPos] == 0)
                            direction = UP;
                    }
                    if (rand() % 4 == 0)
                    {
                        if (board[xPos - 1][yPos] != 0)
                            direction = DOWN;
                        else
                            direction = UP;
                    }
                    else
                    {
                        if (board[xPos + 1][yPos] != 0)
                            direction = UP;
                        else
                            direction = DOWN;
                    }
                    break;

                case LEFT:
                    if (!isSafe)
                    {
                        if (board[xPos][yPos + 1] == 0)
                            direction = RIGHT;
                        else if (board[xPos + 1][yPos] == 0)
                            direction = DOWN;
                        else if (board[xPos - 1][yPos] == 0)
                            direction = UP;
                    }
                    if (rand() % 4 == 0)
                    {
                        if (board[xPos - 1][yPos] != 0)
                            direction = DOWN;
                        else
                            direction = UP;
                    }
                    else
                    {
                        if (board[xPos + 1][yPos] != 0)
                            direction = UP;
                        else
                            direction = DOWN;
                    }
                    break;

                case UP:
                    if (!isSafe)
                    {
                        if (board[xPos - 1][yPos] == 0)
                            direction = DOWN;
                        else if (board[xPos][yPos + 1] == 0)
                            direction = RIGHT;
                        else if (board[xPos][yPos - 1] == 0)
                            direction = LEFT;
                    }
                    if (rand() % 4 == 0)
                    {
                        if (board[xPos][yPos - 1] != 0)
                            direction = RIGHT;
                        else
                            direction = LEFT;
                    }
                    else
                    {
                        if (board[xPos][yPos + 1] != 0)
                            direction = LEFT;
                        else
                            direction = RIGHT;
                    }
                    break;

                case DOWN:
                    if (!isSafe)
                    {
                        if (board[xPos - 1][yPos] == 0)
                            direction = UP;
                        else if (board[xPos][yPos + 1] == 0)
                            direction = LEFT;
                        else if (board[xPos][yPos - 1] == 0)
                            direction = RIGHT;
                    }
                    if (rand() % 4 == 0)
                    {
                        if (board[xPos][yPos - 1] != 0)
                            direction = RIGHT;
                        else
                            direction = LEFT;
                    }
                    else
                    {
                        if (board[xPos][yPos + 1] != 0)
                            direction = LEFT;
                        else
                            direction = RIGHT;
                    }
                    break;
                }
            }
        } while (!isSafe);
    }
}
