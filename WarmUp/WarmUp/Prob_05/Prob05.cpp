#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <deque>
#include <math.h>
#include <algorithm>

#define NONE 100
#define INF 9999

using namespace std;

typedef struct Element
{
    int x, y, z;
    int idx;
} Element;

deque<Element> list;
deque<Element> sortedList;

void listPrint();

void listAddTop(Element);
void listAddBottom(Element);
void listRemoveTop();
void listRemoveBottom();
void listLongest();
void listShortest();

void listSort();
bool isSorted = false;

float pointLength(Element);

bool comp(Element, Element);

int main()
{
    char commend = -1;
    Element number;

    while (commend != 'q')
    {
        number = {-1, -1, -1, -1};
        cout << "Commend : ";
        cin >> commend;

        switch (commend)
        {
        case '+':
        case 'e':
            scanf(" %d %d %d", &number.x, &number.y, &number.z);
            if (list.size() == 10)
            {
                cout << "List is full\n";
                break;
            }
            if (commend == '+')
                listAddTop(number);
            else if (commend == 'e')
                listAddBottom(number);
            if (isSorted)
                listSort();
            listPrint();
            break;

        case '-':
            listRemoveTop();
            if (isSorted)
                listSort();
            listPrint();
            break;
        case 'd':
            listRemoveBottom();
            if (isSorted)
                listSort();
            listPrint();
            break;

        case 'l':
            cout << "Count : " << list.size() << "\n";
            break;
        case 'c':
            list.clear();
            listPrint();
            break;
        case 'm':
            listLongest();
            break;
        case 'n':
            listShortest();
            break;

        case 's':
            if (!isSorted)
            {
                isSorted = true;
                listSort();
            }
            else
                isSorted = false;
            listPrint();
            break;

        default:
            break;
        }
    }
}

void listPrint()
{
    Element buffer[10];
    for (int i = 0; i < 10; ++i)
    {
        buffer[i].idx = NONE;
    }

    if (!isSorted)
    {
        for (int i = 0; i < list.size(); ++i)
        {
            buffer[list[i].idx] = list[i];
        }
    }
    else
    {
        for (int i = 0; i < sortedList.size(); ++i)
        {
            buffer[i] = sortedList[i];
        }
    }

    cout << "No. x  y  z   L\n";

    for (int i = 9; i >= 0; --i)
    {
        cout << i << "  ";
        if (buffer[i].idx == NONE)
            cout << "--------------";
        else
        {
            cout << " " << buffer[i].x << "  " << buffer[i].y << "  " << buffer[i].z;
            printf("  %2.1f", pointLength(buffer[i]));
            // printf("  %2.1f", sqrt(pow(buffer[i].x, 2) + pow(buffer[i].y, 2) + pow(buffer[i].z, 2)));
            //  cout << "  " << sqrt(pow(buffer[i].x, 2) + pow(buffer[i].y, 2) + pow(buffer[i].z, 2));
        }

        cout << "\n";
    }
}

void listAddTop(Element item)
{
    if (list.empty())
        item.idx = 0;
    else
        item.idx = (list.back().idx + 1) % 10;

    if (item.idx == 0)
        list.push_front(item);
    else
        list.push_back(item);
}

void listAddBottom(Element item)
{
    if (list.empty())
        item.idx = 0;
    else
    {
        if (list.front().idx == 0)
        {
            for (int i = 0; i < list.size(); ++i)
            {
                ++list[i].idx;
            }
            item.idx = 0;
        }
        else
        {
            item.idx = list.front().idx - 1;
        }
    }

    list.push_front(item);
}

void listRemoveTop()
{
    if (list.empty())
        cout << "List is empty\n";
    else
        list.pop_back();
}

void listRemoveBottom()
{
    if (list.empty())
        cout << "List is empty\n";
    else
        list.pop_front();
}

void listLongest()
{
    if (list.empty())
    {
        cout << "List is empty\n";
        return;
    }

    float length = -1;
    Element longest;
    for (int i = 0; i < list.size(); ++i)
    {
        if (length < pointLength(list[i]))
        {
            length = pointLength(list[i]);
            longest = list[i];
        }
    }
    cout << "Longest : (" << longest.x << ", " << longest.y << ", " << longest.z << ") ";
    printf("%2.1f\n", length);
}

void listShortest()
{
    if (list.empty())
    {
        cout << "List is empty\n";
        return;
    }

    float length = INF;
    Element shortest;
    for (int i = 0; i < list.size(); ++i)
    {
        if (length > pointLength(list[i]))
        {
            length = pointLength(list[i]);
            shortest = list[i];
        }
    }
    cout << "Shortest : (" << shortest.x << ", " << shortest.y << ", " << shortest.z << ") ";
    printf("%2.1f\n", length);
}

void listSort()
{
    sortedList = list;
    sort(sortedList.begin(), sortedList.end(), comp);
}

float pointLength(Element item)
{
    return sqrt(pow(item.x, 2) + pow(item.y, 2) + pow(item.z, 2));
}

bool comp(const Element item1, const Element item2)
{

    if (pointLength(item1) < pointLength(item2))
        return true;
    else
        return false;
}