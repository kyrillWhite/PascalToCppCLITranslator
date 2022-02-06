#include <iostream>
#include "CIO.h"

using namespace std;

int main()
{
    ifstream input("input.txt");
    ofstream output("output.txt");
    auto IO = make_unique<CIO>(input, output);
    char ch;
    while (IO->GetNextChar(ch)) {
        cout << ch << endl;
    }
    IO->PrintErrors();
}