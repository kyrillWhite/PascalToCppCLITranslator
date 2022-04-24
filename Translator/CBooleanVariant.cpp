#include "CBooleanVariant.h"

CBooleanVariant::CBooleanVariant(bool _value) : CVariant(vtBoolean), value(_value) { }

bool CBooleanVariant::GetValue()
{
    return value;
}

std::string CBooleanVariant::ToString()
{
    return value ? "true" : "false";
}
