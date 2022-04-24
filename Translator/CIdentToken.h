#pragma once
#include "CToken.h"

class CIdentToken : public CToken
{
private:
    std::string name;

public:
    CIdentToken(std::string _name);
    std::string ToString() override;
};