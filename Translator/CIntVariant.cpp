#include "CIntVariant.h"

CIntVariant::CIntVariant(int _value) : CVariant(vtInt), value(_value) { }

int CIntVariant::GetValue()
{
    return value;
}

std::string CIntVariant::ToString()
{
    return std::to_string(value);
}
