#pragma once
#include <string>

#include "EErrorType.h"

class CError
{
private:
    int lineNumber;
    int symbolNumber;
    ErrorType eType;

public:
    CError(int _lineNumber, int _symbolNumber, ErrorType _eType);
    int GetLineNum();
    int GetSymbolNum();
    std::string GetText();
};