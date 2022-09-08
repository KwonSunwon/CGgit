#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

int wordCounter(string[]);
int numberCounter(string[]);
int capitalCounter(string[]);

int main()
{
    ifstream file;
    file.open("data.txt", ifstream::in);

    string sentence[10];

    for (int i = 0; i < 10; ++i)
    {
        getline(file, sentence[i]);
    }
    file.close();

    for (int i = 0; i < 10; ++i)
        cout << sentence[i] << "\n";
    cout << "\n";
    cout << "word count : " << wordCounter(sentence) << "\n";
    cout << "number count : " << numberCounter(sentence) << "\n";
    cout << "Capital word : " << capitalCounter(sentence);
}

// 나중에 3D 같은 단어의 제대로된 처리해야됨
// 대문자 개수 카운트한 거처럼 플래그 활용하면 될거 같음
int wordCounter(string str[10])
{
    int wordCnt = 0;
    for (int i = 0; i < 10; ++i)
    {
        int idx = 0;
        while (str[i][idx] != '\0')
        {
            if ((str[i][idx] >= 0x3A && str[i][idx] <= 0x7E) || (str[i][idx] >= 0x21 && str[i][idx] <= 0x2F))
            {
                if (str[i][idx + 1] == ' ' || str[i][idx + 1] == '\0')
                    ++wordCnt;
            }
            ++idx;
        }
    }
    return wordCnt;
}

int numberCounter(string str[10])
{
    int numberCnt = 0;

    for (int i = 0; i < 10; ++i)
    {
        int idx = 0;
        while (str[i][idx] != '\0')
        {
            if (str[i][idx] >= 0x30 && str[i][idx] <= 0x39)
            {
                if (str[i][idx + 1] == ' ' || str[i][idx + 1] == '\0')
                    ++numberCnt;
            }
            ++idx;
        }
    }

    return numberCnt;
}

int capitalCounter(string str[10])
{
    int capitalCnt = 0;
    bool isCap = false;

    for (int i = 0; i < 10; ++i)
    {
        int idx = 0;
        while (str[i][idx] != '\0')
        {
            if (isupper(str[i][idx]))
                isCap = true;
            if (str[i][idx + 1] == ' ' || str[i][idx + 1] == '\0')
                if (isCap)
                {
                    ++capitalCnt;
                    isCap = false;
                }
            ++idx;
        }
        ++idx;
    }
    return capitalCnt;
}