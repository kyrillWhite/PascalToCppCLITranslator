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

    auto token = lexer->GetNextToken();
    while (token->GetType() != ttEmpty) {
        cout << token->ToString() << endl;
        token = lexer->GetNextToken();
    }
    lexer->GetIOPtr()->PrintErrors();
    input.close();
    //output.close();
}