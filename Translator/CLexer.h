#pragma once
#include <map>

#include "CIO.h"
#include "CToken.h"
#include "CKeyWordToken.h"
#include "CIdentToken.h"
#include "CConstToken.h"
#include "CEmptyToken.h"
#include "EKeyWordType.h"

class CLexer
{
private:
    char ch;
    std::unique_ptr<CIO> IO;
    std::map<std::string, EKeyWordType> keyWords;

public:
    CLexer(std::unique_ptr<CIO> _IO);
    CIO* GetIOPtr();
    std::unique_ptr<CToken> GetNextToken();
};