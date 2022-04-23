#include "CVariant.h"

CVariant::CVariant(EVariantType _variantType) 
    : variantType(_variantType) { }

EVariantType CVariant::GetVariantType()
{
    return variantType;
}

CVariant::~CVariant() { }
