#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

using namespace std;

RECT rect1, rect2;

void rectPrint();
void rectInit();

int main()
{
}

void rectPrint()
{
    cout << "Rect 1 : (" << rect1.left << ", " << rect1.top << ") (" << rect1.right << ", " << rect1.bottom << "\n";
    cout << "Rect 2 : (" << rect2.left << ", " << rect2.top << ") (" << rect2.right << ", " << rect2.bottom << "\n";
}

void rectInit()
{
}
