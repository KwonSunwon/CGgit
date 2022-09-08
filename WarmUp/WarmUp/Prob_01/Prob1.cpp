#include <iostream>
#include <random>
#include <time.h>

#define INF 100
#define ADD 200
#define MUL 300
#define SUB 400
#define EQU 500
#define TRANS 600

using namespace std;

int mat1[3][3], mat2[3][3];
int result[3][3];
int tranResult[4][4];
int printMat[4][11];
int detResult1, detResult2;

void print(char);

void mul(); // m
void add(); // a
void sub(); // d
void det(); // r

void init(); // s

int main()
{
    char order = -1;

    init();

    while (order != 'q')
    {
        cout << "input : ";
        cin >> order;

        switch (order)
        {
        case 'm':
            mul();
            print(order);
            break;
        case 'a':
            add();
            print(order);
            break;
        case 'd':
            sub();
            print(order);
            break;

        case 'r':
            det();
            print(order);
            break;

        case 't':
            print(order);
            break;

        case 'h':
            print(order);
            break;

        case 's':
            init();
            break;
        }
    }
}

void print(char type)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 11; ++j)
        {
            printMat[i][j] = INF;
        }

    switch (type)
    {
    case 'm':
    case 'a':
    case 'd':
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                printMat[i][j] = mat1[i][j];
                printMat[i][j + 4] = mat2[i][j];
                printMat[i][j + 8] = result[i][j];
            }

        if (type == 'm')
            printMat[1][3] = MUL;
        else if (type == 'a')
            printMat[1][3] = ADD;
        else if (type == 'd')
            printMat[1][3] = SUB;

        printMat[1][7] = EQU;

        for (int i = 0; i < 3; ++i)
        {
            cout << '|';
            for (int j = 0; j < 11; ++j)
            {
                if (j == 3 || j == 4 || j == 7 || j == 8)
                    cout << '|';

                if (printMat[i][j] == INF)
                    cout << ' ';
                else if (printMat[i][j] == MUL)
                    cout << '.';
                else if (printMat[i][j] == ADD)
                    cout << '+';
                else if (printMat[i][j] == SUB)
                    cout << '-';
                else if (printMat[i][j] == EQU)
                    cout << '=';
                else
                    printf("%2d", printMat[i][j]);
            }
            cout << "|\n";
        }
        break;

    case 'r':
        printMat[1][3] = EQU;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                printMat[i][j] = mat1[i][j];
            }

        printMat[1][4] = detResult1;
        for (int k = 0; k < 2; ++k)
        {
            for (int i = 0; i < 3; ++i)
            {
                cout << '|';
                for (int j = 0; j < 5; ++j)
                {
                    if (j == 3)
                        cout << '|';

                    if (printMat[i][j] == INF)
                        cout << ' ';
                    else if (printMat[i][j] == EQU)
                        cout << '=';
                    else
                        printf("%2d", printMat[i][j]);
                }
                cout << "\n";
            }
            cout << '\n';
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                {
                    printMat[i][j] = mat2[i][j];
                }

            printMat[1][4] = detResult2;
        }
        break;

    case 't':
        det();
        printMat[1][3] = TRANS;
        printMat[1][8] = EQU;

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                printMat[i][j] = mat1[i][j];
                printMat[i][j + 4] = mat1[j][i];
            }
        printMat[1][9] = detResult1;

        for (int i = 0; i < 3; ++i)
        {
            cout << '|';
            for (int j = 0; j < 11; ++j)
            {
                if (j == 3 || j == 4 || j == 7)
                    cout << '|';

                if (printMat[i][j] == INF)
                    cout << ' ';
                else if (printMat[i][j] == TRANS)
                    cout << '>';
                else if (printMat[i][j] == EQU)
                    cout << '=';
                else
                    printf("%2d", printMat[i][j]);
            }
            cout << "\n";
        }
        cout << "\n";

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                printMat[i][j] = mat2[i][j];
                printMat[i][j + 4] = mat2[j][i];
            }
        printMat[1][9] = detResult2;

        for (int i = 0; i < 3; ++i)
        {
            cout << '|';
            for (int j = 0; j < 11; ++j)
            {
                if (j == 3 || j == 4 || j == 7)
                    cout << '|';

                if (printMat[i][j] == INF)
                    cout << ' ';
                else if (printMat[i][j] == TRANS)
                    cout << '>';
                else if (printMat[i][j] == EQU)
                    cout << '=';
                else
                    printf("%2d", printMat[i][j]);
            }
            cout << "\n";
        }
        break;

    case 'h':
        det();
        printMat[1][3] = TRANS;
        printMat[1][9] = EQU;

        for (int i = 0; i < 3; ++i)
        {
            printMat[3][i + 4] = 0;
            printMat[i][7] = 0;
        }
        printMat[3][7] = 1;

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                printMat[i][j] = mat1[i][j];
                printMat[i][j + 4] = mat1[i][j];
            }
        printMat[1][10] = detResult1;

        for (int i = 0; i < 4; ++i)
        {
            if (i < 3)
                cout << '|';
            if (i == 3)
                cout << "     ";
            for (int j = 0; j < 11; ++j)
            {
                if ((j == 3 && i < 3) || j == 4 || j == 8)
                    cout << '|';

                if (printMat[i][j] == INF)
                    cout << ' ';
                else if (printMat[i][j] == TRANS)
                    cout << '>';
                else if (printMat[i][j] == EQU)
                    cout << '=';
                else
                    printf("%2d", printMat[i][j]);
            }
            cout << "\n";
        }
        cout << "\n";

        for (int i = 0; i < 3; ++i)
        {
            printMat[3][i + 4] = 0;
            printMat[i][7] = 0;
        }
        printMat[3][7] = 1;

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                printMat[i][j] = mat2[i][j];
                printMat[i][j + 4] = mat2[i][j];
            }
        printMat[1][10] = detResult2;

        for (int i = 0; i < 4; ++i)
        {
            if (i < 3)
                cout << '|';
            if (i == 3)
                cout << "     ";
            for (int j = 0; j < 11; ++j)
            {
                if ((j == 3 && i < 3) || j == 4 || j == 8)
                    cout << '|';

                if (printMat[i][j] == INF)
                    cout << ' ';
                else if (printMat[i][j] == TRANS)
                    cout << '>';
                else if (printMat[i][j] == EQU)
                    cout << '=';
                else
                    printf("%2d", printMat[i][j]);
            }
            cout << "\n";
        }
        break;
    }

    cout << '\n';
}

void init()
{
    srand(time(NULL));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            mat1[i][j] = rand() % 3;
            mat2[i][j] = rand() % 3;
        }
}

void mul()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            result[i][j] = mat1[i][0] * mat2[0][j] + mat1[i][1] * mat2[1][j] + mat1[i][2] * mat2[2][j];
        }
}

void add()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result[i][j] = mat1[i][j] + mat2[i][j];
}

void sub()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result[i][j] = mat1[i][j] - mat2[i][j];
}

void det()
{
    detResult1 = mat1[0][0] * mat1[1][1] * mat1[2][2] + mat1[0][1] * mat1[1][2] * mat1[2][0] + mat1[0][2] * mat1[1][0] * mat1[2][1] - mat1[0][2] * mat1[1][1] * mat1[2][0] - mat1[0][1] * mat1[1][0] * mat1[2][2] - mat1[0][0] * mat1[1][2] * mat1[2][1];
    detResult2 = mat2[0][0] * mat2[1][1] * mat2[2][2] + mat2[0][1] * mat2[1][2] * mat2[2][0] + mat2[0][2] * mat2[1][0] * mat2[2][1] - mat2[0][2] * mat2[1][1] * mat2[2][0] - mat2[0][1] * mat2[1][0] * mat2[2][2] - mat2[0][0] * mat2[1][2] * mat2[2][1];
}
