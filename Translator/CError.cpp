#include "CError.h"

using namespace std;

CError::CError(int _lineNumber, int _symbolNumber, ErrorType _eType) :
    lineNumber(_lineNumber), symbolNumber(_symbolNumber), eType(_eType) { }

int CError::GetLineNum()
{
    return lineNumber;
}

int CError::GetSymbolNum()
{
    return symbolNumber;
}

string CError::GetText()
{
    switch (eType) {
    default:
        return "";
    }
}
