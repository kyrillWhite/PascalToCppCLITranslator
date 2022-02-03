#include "CIO.h"
#include <iostream>

using namespace std;

int main()
{
    ifstream input("input.txt");
    ofstream output("output.txt");
    auto IO = make_unique<CIO>(input, output);
    char ch;
    while ((*IO).GetNextChar(ch)) {
        cout << ch << endl;
    }
}