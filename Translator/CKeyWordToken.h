#pragma once
#include "CToken.h"
#include "EKeyWordType.h"
class CKeyWordToken : public CToken
{
private:
    EKeyWordType keyWordType;
public:
    CKeyWordToken(EKeyWordType _keyWordType);
    EKeyWordType GetKeyWordType();
    std::string ToString() override;
};

