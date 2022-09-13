#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>

using namespace std;

// Board
#define BLNK 500
#define RED_A 100
#define RED_B 101
#define BLUE_A 200
#define BLUE_B 201
#define GREEN_A 300
#define GREEN_B 301
#define YELLOW_A 400
#define YEELOW_B 401

#define LIFE 10

pair<int, bool> board[5][5];
int score;
int life;

pair<int, int> point[2];

void boardInit();
void cardInit();
void init();

bool isSame(const pair<int, int>, const pair<int, int>);
bool isGameWin();

void draw();

int main()
{
    char commend = -1;
    int number = -1;
    bool isReset = false;
    bool isGameEnd = false;
    bool isSafe = true;

    do
    {
        init();
        draw();

        while (1)
        {
            do
            {
                isSafe = true;
                cout << "input card 1 (or commend) : ";
                cin >> commend;

                if (commend == 'r')
                {
                    isReset = true;
                    break;
                }
                else if (commend == 'q')
                {
                    isGameEnd = true;
                    break;
                }
                cin >> number;

                point[0] = {number, commend - 96};
                if (board[point[0].first][point[0].second].second)
                    isSafe = false;
                else
                    board[point[0].first][point[0].second].second = true;
                draw();
                if (!isSafe)
                    cout << "Is already open\n";
            } while (!isSafe && !isGameEnd);

            if (isGameEnd)
                break;

            do
            {
                isSafe = true;
                cout << "input card 2 : ";
                cin >> commend;
                cin >> number;

                point[1] = {number, commend - 96};
                if (board[point[1].first][point[1].second].second || point[0] == point[1])
                    isSafe = false;
                else
                    board[point[1].first][point[1].second].second = true;
                draw();
                if (!isSafe)
                    cout << "Is already open\n";
            } while (!isSafe);

            if (isSame(point[0], point[1]))
            {
                ++score;
                if (isGameWin())
                    isGameEnd = true;
            }
            else
            {
                board[point[0].first][point[0].second].second = false;
                board[point[1].first][point[1].second].second = false;
                if (--life == 0)
                {
                    cout << "Game Over...\n";
                    cout << "Your Score : " << score * 100 << "\n";
                    system("PAUSE");
                    break;
                }
                else
                {
                    system("PAUSE");
                    draw();
                }
            }

            if (isGameEnd)
                break;
        }
    } while (isReset && !isGameEnd);

    draw();
    if (isGameWin())
    {
        cout << "YOU WIN!\n";
        cout << score * 100 + life * 10 << "\n";
    }
    else
    {
        cout << "Game Over...\n";
        cout << "Your Score : " << score * 100;
    }
}

void init()
{
    boardInit();
    cardInit();
    score = 0;
    life = LIFE;
    for (int i = 0; i < 2; ++i)
        point[i] = {-1, -1};
}

void boardInit()
{
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        {
            board[i][j].first = BLNK;
            board[i][j].second = false;
        }
}

void draw()
{
    system("cls");
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (i == 0 && j == 0)
                cout << " ";
            else if (i == 0)
                cout << (char)(96 + j);
            else if (j == 0)
                cout << i;
            else if (!board[i][j].second)
                cout << "*";
            else if (board[i][j].second)
            {
                switch (board[i][j].first / 100)
                {
                case 1:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                    break;
                case 2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                    break;
                case 3:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                    break;
                case 4:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
                    break;
                }
                if (board[i][j].first % 2 == 0)
                    cout << "A";
                else if (board[i][j].first % 2 == 1)
                    cout << "B";
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout << " ";
        }
        cout << "\n";
    }
    for (int i = 0; i < life; ++i)
        cout << "<3 ";
    cout << "\n";
}

void cardInit()
{
    vector<int> deck;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                deck.push_back((i + 1) * 100 + j);
    shuffle(deck.begin(), deck.end(), default_random_engine(time(NULL)));

    for (int i = 1; i < 5; ++i)
        for (int j = 1; j < 5; ++j)
        {
            board[i][j].first = deck.back();
            deck.pop_back();
            // Test
            // board[i][j].second = true;
            cout << board[i][j].first << " ";
        }
    system("PAUSE");
}

bool isSame(const pair<int, int> card1, const pair<int, int> card2)
{
    if (board[card1.first][card1.second] == board[card2.first][card2.second])
        return true;
    else
        return false;
}

bool isGameWin()
{
    for (int i = 1; i < 5; ++i)
        for (int j = 1; j < 5; ++j)
            if (!board[i][j].second)
                return false;
    return true;
}