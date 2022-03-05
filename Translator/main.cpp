#include <iostream>
#include "CIO.h"
#include "CLexer.h"

using namespace std;

int main()
{
    setlocale(LC_CTYPE, "Russian");
    ifstream input("input.txt");
    //ofstream output("output.txt");
    auto IO = make_unique<CIO>(input, cout);
    auto lexer = make_unique<CLexer>(move(IO));
    
    try {
        while (!lexer->GetIOPtr()->IsEndOfInput()) {
            auto token = lexer->GetNextToken();
            cout << token->ToString() << endl;
        }
    }
    catch (CError* error) {
        lexer->GetIOPtr()->PrintErrors();
    }

    input.close();
    //output.close();
}