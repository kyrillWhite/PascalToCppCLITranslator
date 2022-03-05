#pragma once
#include "CVariant.h"

class CBooleanVariant : public CVariant
{
private:
    bool value;

public:
    CBooleanVariant(bool _value);
    bool GetValue();
    std::string ToString() override;
};