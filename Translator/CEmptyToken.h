#pragma once
#include "CToken.h"

class CEmptyToken : public CToken
{
public:
    CEmptyToken();
    std::string ToString() override;
};

