/*=============================================================================
| Assignment: pa02 - Calculating an 8, 16, or 32 bit
| checksum on an ASCII input file
|
| Author: Feifan Cao
| Language: c, c++, Java, GO, Python
|
| To Compile: javac pa02.java
| gcc -o pa02 pa02.c
| g++ -o pa02 pa02.cpp
| go build pa02.go
| python pa02.py //Caution - expecting input parameters
|
| To Execute: java -> java pa02 inputFile.txt 8
| or c++ -> ./pa02 inputFile.txt 8
| or c -> ./pa02 inputFile.txt 8
| or go -> ./pa02 inputFile.txt 8
| or python-> python pa02.py inputFile.txt 8
| where inputFile.txt is an ASCII input file
| and the number 8 could also be 16 or 32
| which are the valid checksum sizes, all
| other values are rejected with an error message
| and program termination
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

string readInput(fstream *f);

int main(int argc, char **argv)
{
    string s2 = argv[2];

    if (s2 != "8" && s2 != "16" && s2 != "32")
        fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");

    fstream inputFile;
    string s1 = argv[1];

    inputFile.open(s1, ios::in);

    if (inputFile.is_open())
    {

        readInput(&inputFile);
        inputFile.close();
    }
    else
    {
        cout << "error opening file or the wrong file name";
        exit(-1);
    }

    return 0;
}

string readInput(fstream *f)
{
    string line;
    while (getline(*f, line))
    {
        cout << line.size() << endl;
    }

}

/*=============================================================================
| I [Feifan Cao] ([5062145]) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+============================================================================*/