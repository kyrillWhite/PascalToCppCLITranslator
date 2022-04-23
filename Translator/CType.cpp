#include "CType.h"

CType::CType(ETypeType _type) : type(_type) { }

ETypeType CType::GetType()
{
    return type;
}

CType::~CType() { }