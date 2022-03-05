#include "CStringVariant.h"

CStringVariant::CStringVariant(std::string _value) : CVariant(vtString), value(_value) { }

std::string CStringVariant::GetValue()
{
    return value;
}

std::string CStringVariant::ToString()
{
    return value;
}
