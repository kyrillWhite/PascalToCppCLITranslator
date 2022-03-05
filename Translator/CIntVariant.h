#pragma once
#include "CVariant.h"

class CIntVariant : public CVariant
{
private:
    int value;

public:
    CIntVariant(int _value);
    int GetValue();
    std::string ToString() override;
};