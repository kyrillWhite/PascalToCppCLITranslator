#include "CRealVariant.h"

CRealVariant::CRealVariant(double _value) : CVariant(vtReal), value(_value) { }

double CRealVariant::GetValue()
{
    return value;
}

std::string CRealVariant::ToString()
{
    std::string str = std::to_string(value);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    return str;
}
