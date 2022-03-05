#include "CVariant.h"

CVariant::CVariant(EVariantType _variantType) 
    : variantType(_variantType) { }

EVariantType CVariant::getVariantType()
{
    return variantType;
}

CVariant::~CVariant() { }
