#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <random>
#include <time.h>
#include <deque>

#define LEFT 0b0001
#define RIGHT 0b0010
#define UP 0b0100
#define DOWN 0b1000

using namespace std;

// int board[30][30];

deque<int> board[30];

void boardPrint();
void boardInit();
void boardMakeRoute();

void boardMoveRight();
void boardMoveLeft();

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
            boardInit();
            boardMakeRoute();
            boardPrint();
            break;
        case 'a': // move route left
            boardMoveLeft();
            boardPrint();
            break;
        case 'd': // move route right
            boardMoveRight();
            boardPrint();
            break;
        }
    }
}

void boardPrint()
{
    system("cls");
    for (int i = 0; i < 30; ++i)
    {
        for (int j = 0; j < 30; ++j)
        {
            printf("%3d ", board[i][j]);
        }
        cout << "\n";
    }
}

void boardInit()
{
    for (int i = 0; i < 30; ++i)
        board[i].clear();
    for (int i = 0; i < 30; ++i)
        for (int j = 0; j < 30; ++j)
        {
            board[i].push_back(0);
        }
}

void boardMakeRoute()
{
    // 조건 1 : 한 방향으로 최대 8칸 이동
    // 조건 2 : 경로를 좌우상하로 최소 한 번 이동
    // 조건 3 : 한 번 지나갔던 길을 지나면 가장 마지막의 값으로 출력

    srand(time(NULL));

    int count = 1;
    int xPos = 0;
    int yPos = 0;
    int direction = RIGHT;
    int lastDirection = -1;

    int leftCnt = 0;
    int upCnt = 0;

    int conditionCount = 0;
    int conditionDirection = 0;

    bool isSafe;
    bool isSameDirection;

    board[0][0] = 1;

    while (1)
    {
        switch (direction)
        {
        case RIGHT:
            ++yPos;
            conditionDirection |= RIGHT;
            break;

        case LEFT:
            --yPos;
            conditionDirection |= LEFT;
            break;

        case UP:
            --xPos;
            conditionDirection |= UP;
            break;

        case DOWN:
            ++xPos;
            conditionDirection |= DOWN;
            break;
        }

        if (xPos < 0 || xPos > 29 || yPos < 0 || yPos > 29)
        {
            return;
        }

        board[xPos][yPos] = ++count;
        lastDirection = direction;
        if (xPos == 29 && yPos == 29 && conditionDirection == 0b1111)
            break;
        ++conditionCount;

        if (rand() % 10 == 0 || conditionCount == 7)
        {
            switch (direction)
            {
            case RIGHT:
                if (rand() % 4 == 0)
                    direction = UP;
                else
                    direction = DOWN;

                break;

            case LEFT:
                if (rand() % 4 == 0)
                    direction = UP;
                else
                    direction = DOWN;

                break;

            case UP:
                if (rand() % 4 == 0)
                    direction = LEFT;
                else
                    direction = RIGHT;
                break;

            case DOWN:
                if (rand() % 4 == 0)
                    direction = LEFT;
                else
                    direction = RIGHT;
                break;
            }
        }

        switch (direction)
        {
        case RIGHT:
            if (yPos + 1 >= 30)
            {
                switch (lastDirection)
                {
                case RIGHT:
                    direction = DOWN;
                    break;
                case DOWN:
                case UP:
                    direction = LEFT;
                    break;
                }
            }
            break;
        case LEFT:
            if (yPos - 1 <= -1)
            {
                switch (lastDirection)
                {
                case LEFT:
                    direction = DOWN;
                    break;
                case DOWN:
                case UP:
                    direction = RIGHT;
                    break;
                }
            }
            break;
        case UP:
            if (xPos - 1 <= -1)
            {
                switch (lastDirection)
                {
                case RIGHT:
                case LEFT:
                    direction = DOWN;
                    break;
                case UP:
                    direction = RIGHT;
                    break;
                }
            }
            break;
        case DOWN:
            if (xPos + 1 >= 30)
            {
                switch (lastDirection)
                {
                case RIGHT:
                case LEFT:
                    direction = UP;
                    break;
                case DOWN:
                    direction = RIGHT;
                    break;
                }
            }
            break;
        }

        if (conditionCount == 7)
            conditionCount = 0;
    }
}

void boardMoveRight()
{
    for (int i = 0; i < 30; ++i)
    {
        board[i].push_front(board[i].back());
        board[i].pop_back();
    }
}

void boardMoveLeft()
{
    for (int i = 0; i < 30; ++i)
    {
        board[i].push_back(board[i].front());
        board[i].pop_front();
    }
}