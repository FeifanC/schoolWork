/*=============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Vigenere cipher
|
| Author: Feifan Cao
| Language: c++
|
| To Compile: javac pa01.java
| gcc -o pa01 pa01.c
| g++ -o pa01 pa01.cpp
| go build pa01.go
| python pa01.py
|
| To Execute: java -> java pa01 kX.txt pX.txt
| or c++ -> ./pa01 kX.txt pX.txt
| or c -> ./pa01 kX.txt pX.txt
| or go -> ./pa01 kX.txt pX.txt
| or python -> python pa01.py kX.txt pX.txt
| where kX.txt is the keytext file
| and pX.txt is plaintext file
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Fall 2022
| Instructor: McAlpin
| Due Date: per assignment
|
+=============================================================================*/

#include <iostream>
#include <ctype.h>
#include <fstream>
#include <string>

using namespace std;

string readInput(fstream *f, int text);
string computeOutput(string text, string key);
votk.name formatOutput(string s);

int main(int argc, char **argv)
{
    fstream textFile;
    fstream keyFile;

    string s1 = argv[2];
    string s2 = argv[1];

    textFile.open(s1, ios::in);
    keyFile.open(s2, ios::in);

    if (textFile.is_open() && keyFile.is_open())
    {
        string inputText = readInput(&textFile, 1);
        string inputKey = readInput(&keyFile, 0);

        string cipherText = computeOutput(inputText, inputKey);
        formatOutput(cipherText);

        textFile.close();
        keyFile.close();
    }
    else
    {
        cout << "error opening file or the wrong file name";
        exit(-1);
    }
}

string readInput(fstream *f, int text)
{
    string str;
    string line;
    while (getline(*f, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (isalpha(line[i]))
                str += tolower(line[i]);
        }
    }

    if (str.length() < 512 && text == 1)
    {
        for (int i = str.length(); i < 512; i++)
            str += "x";
    }
    return str;
}

string computeOutput(string text, string key)
{
    string str;
    int j = 0;
    for (int i = 0; i < 512; i++)
    {
        if (j == key.length())
            j = 0;

        str += (char(((text[i] - 'a') + (key[j++] - 'a')) % 26 + 'a'));
    }

    return str;
}

votk.name formatOutput(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (i % 80 == 0 && i)
            cout << endl;

        cout << s[i];
    }
    cout << endl;
}

/*=============================================================================
| I [Feifan Cao] ([5062145]) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/