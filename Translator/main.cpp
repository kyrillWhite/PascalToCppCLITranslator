#include <iostream>
#include "CParser.h"

using namespace std;

int main()
{
    setlocale(LC_CTYPE, "Russian");
    ifstream input("input.txt");
    //ofstream output("output.txt");
    auto IO = make_unique<CIO>(input, cout);
    auto lexer = make_unique<CLexer>(move(IO));
    auto parser = make_unique<CParser>(move(lexer));
    
    try {
        parser->Evaluate();
    }
    catch (...) { }

    parser->GetLexerPtr()->GetIOPtr()->PrintErrors();
    input.close();
    //output.close();
}