#pragma once
#include <map>

#include "CIO.h"
#include "CKeyWordToken.h"
#include "CIdentToken.h"
#include "CConstToken.h"
#include "CEmptyToken.h"
#include "EKeyWordType.h"

class CLexer
{
private:
    char ch;
    std::shared_ptr<CIO> IO;
    std::map<std::string, EKeyWordType> keyWords;

public:
    CLexer(std::shared_ptr<CIO> _IO);
    std::shared_ptr<CIO> GetIOPtr();
    std::unique_ptr<CToken> GetNextToken();
};