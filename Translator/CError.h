#pragma once
#include <string>
#include <exception>

#include "EErrorType.h"

class CError : public std::exception
{
private:
    int lineNumber;
    int symbolNumber;
    EErrorType eType;

public:
    CError(int _lineNumber, int _symbolNumber, EErrorType _eType);
    int GetLineNum();
    int GetSymbolNum();
    std::string GetText();
};