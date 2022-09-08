#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;

void sentencePrint();

void flip();
void insert();
void smallFlip();
void alt();
void decal();

bool isInsertion = false;
string sentence[10];

int main()
{
    ifstream file;
    file.open("data.txt", ifstream::in);

    for (int i = 0; i < 10; ++i)
    {
        getline(file, sentence[i]);
    }
    file.close();

    char order = -1;

    while (order != 'q')
    {
        cout << "input : ";
        cin >> order;

        switch (order)
        {
        case 'd':
            flip();
            break;
        case 'e':
            insert();
            break;
        case 'f':
            smallFlip();
            break;
        case 'g':
            alt();
            break;
        case 'h':
            decal();
            break;

        default:
            break;
        }

        if (order != 'h' && order != 'q')
            sentencePrint();
    }
}

void sentencePrint()
{
    if (isInsertion)
    {
        for (int i = 0; i < 10; ++i)
        {
            int idx = 0;
            while (sentence[i][idx] != '\0')
            {
                if (idx != 0 && idx % 3 == 0)
                    cout << "@@";
                cout << sentence[i][idx];
                ++idx;
            }
            cout << "\n";
        }
    }
    else
    {
        for (int i = 0; i < 10; ++i)
            cout << sentence[i] << "\n";
    }
}

void flip()
{
    for (int i = 0; i < 10; ++i)
    {
        reverse(sentence[i].begin(), sentence[i].end());
    }
}

void insert()
{
    if (isInsertion)
        isInsertion = false;
    else
        isInsertion = true;
}

void smallFlip()
{
    stack<char> buffer;
    int idx, flag, temp;

    for (int i = 0; i < 10; ++i)
    {
        idx = 0;
        flag = 0;

        while (sentence[i][idx] != '\0')
        {
            buffer.push(sentence[i][idx]);
            ++idx;
            if (sentence[i][idx] == ' ' || sentence[i][idx] == '\0')
            {
                for (flag; flag < idx; ++flag)
                {
                    sentence[i][flag] = buffer.top();
                    buffer.pop();
                    if (buffer.empty())
                        break;
                }
                flag = idx + 1;
            }
        }
    }
}

void alt()
{
    char dest, src;
    cout << "Target : ";
    cin >> dest;
    cout << "Source : ";
    cin >> src;

    for (int i = 0; i < 10; ++i)
    {
        int idx = 0;
        while (sentence[i][idx] != '\0')
        {
            if (sentence[i][idx] == dest)
                sentence[i][idx] = src;
            ++idx;
        }
    }
}

void decal()
{
    int left, right;
    string buffer;

    for (int i = 0; i < 10; ++i)
    {
        buffer.clear();
        left = 0;
        right = sentence[i].length() - 1;

        while (left != right)
        {
            if (sentence[i][left] == sentence[i][right])
            {
                buffer += sentence[i][left];
                ++left;
                --right;
            }
            else
                break;
        }
        if (!buffer.empty())
        {
            cout << sentence[i] << " : " << buffer << "\n";
        }
    }
}