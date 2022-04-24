#pragma once
#include "CVariant.h"

class CStringVariant : public CVariant
{
private:
    std::string value;

public:
    CStringVariant(std::string _value);
    std::string GetValue();
    std::string ToString() override;
};