#pragma once
#include "ETokenType.h"
#include <string>
#include <memory>

class CToken
{
private:
    ETokenType tType;

public:
    CToken(ETokenType _tType);
    ETokenType GetType();
    virtual std::string ToString() = 0;
    virtual ~CToken();
};