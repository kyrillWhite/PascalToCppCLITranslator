#pragma once
#include <string>

#include "EVariantType.h"

class CVariant
{
private:
    EVariantType variantType;
public:
    CVariant(EVariantType _variantType);
    EVariantType getVariantType();
    virtual std::string ToString() = 0;
    virtual ~CVariant();
};