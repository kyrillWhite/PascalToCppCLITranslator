#pragma once
#include "CVariant.h"

class CRealVariant : public CVariant
{
private:
    double value;

public:
    CRealVariant(double _value);
    double GetValue();
    std::string ToString() override;
};