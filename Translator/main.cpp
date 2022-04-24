#include <iostream>
#include "CParser.h"

using namespace std;

int main()
{
    setlocale(LC_CTYPE, "Russian");
    ifstream input("input.txt");
    //ofstream output("output.txt");
    ofstream generatedCode("GenCode/clr.cpp");
    auto IO = make_shared<CIO>(input, cout);
    auto lexer = make_shared<CLexer>(IO);
    auto generator = make_shared<CGenerator>(generatedCode);
    auto parser = make_shared<CParser>(lexer, generator, true);
    
    try {
        parser->Evaluate();
    }
    catch (...) { }
    
    parser->GetLexerPtr()->GetIOPtr()->PrintErrors();
    input.close();
    generatedCode.close();
    //output.close();
    return 0;
}