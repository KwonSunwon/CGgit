#include "maze.h"

vector<int> _makeMaze(int row, int col)
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

    vector<int> maze(row * col, 0);

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

        if (xPos < 0 || xPos > col - 1 || yPos < 0 || yPos > row - 1)
        {
            vector<int> empty;
            return empty;
        }

        maze[yPos * row + xPos] = ++count;
        lastDirection = direction;
        if (xPos == col - 1 && yPos == row - 1 && conditionDirection == 0b1111)
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
            if (yPos + 1 >= row)
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
                    if (yPos + 1 >= row)
                        direction = LEFT;
                    else
                        direction = RIGHT;
                    break;
                }
            }
            break;
        case DOWN:
            if (xPos + 1 >= col)
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

    return maze;
}