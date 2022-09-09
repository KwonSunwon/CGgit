#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

#define SPEED 30

using namespace std;

RECT rect[2];
RECT temp;

void rectPrint();
void rectInit();

int main()
{
    rectInit();
    rectPrint();

    char order = -1;

    int turn = 0;
    bool error = false;

    while (order != 'q')
    {
        cout << "Commend : ";
        cin >> order;
        switch (order)
        {
        case 'w':
            OffsetRect(&rect[turn], 0, -SPEED);
            if (rect[turn].top < 0)
            {
                error = true;
                OffsetRect(&rect[turn], 0, SPEED);
            }
            break;
        case 'a':
            OffsetRect(&rect[turn], -SPEED, 0);
            if (rect[turn].left < 0)
            {
                error = true;
                OffsetRect(&rect[turn], SPEED, 0);
            }
            break;
        case 's':
            OffsetRect(&rect[turn], 0, SPEED);
            if (rect[turn].bottom > 600)
            {
                error = true;
                OffsetRect(&rect[turn], 0, -SPEED);
            }
            break;
        case 'd':
            OffsetRect(&rect[turn], SPEED, 0);
            if (rect[turn].right > 800)
            {
                error = true;
                OffsetRect(&rect[turn], -SPEED, 0);
            }
            break;
        }
        turn = (++turn) % 2;

        if (error)
        {
            error = false;
            cout << "Error : Out of range\n";
        }

        rectPrint();
        if (IntersectRect(&temp, &rect[0], &rect[1]))
            cout << "Rectangle 1 & Rectangle 2 collide!\n";
    }
}

void rectPrint()
{
    for (int i = 0; i < 2; ++i)
        cout << "Rect " << i + 1 << " : (" << rect[i].left << ", " << rect[i].top << ") (" << rect[i].right << ", " << rect[i].bottom << ")\n";
}

void rectInit()
{

    srand(time(NULL));
    do
    {
        for (int i = 0; i < 2; ++i)
        {
            RECT bufferRect = {
                rand() % 500,
                rand() % 300,
                (bufferRect.left + rand() % 200) + 50,
                (bufferRect.top + rand() % 200) + 50,
            };
            rect[i] = bufferRect;
        }
    } while (IntersectRect(&temp, &rect[0], &rect[1]));
}
