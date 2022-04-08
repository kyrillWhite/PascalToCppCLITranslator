#include <iostream>
#include "CParser.h"

using namespace std;

int main()
{
    setlocale(LC_CTYPE, "Russian");
    ifstream input("input.txt");
    //ofstream output("output.txt");
    auto IO = make_shared<CIO>(input, cout);
    auto lexer = make_shared<CLexer>(IO);
    auto parser = make_shared<CParser>(lexer);

    try {
        parser->Evaluate();
    }
    catch (...) { }

    parser->GetLexerPtr()->GetIOPtr()->PrintErrors();
    input.close();
    //output.close();
}