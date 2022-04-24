#include "CType.h"

CType::CType(ETypeType _type) : type(_type), ref(false), isIdent(false) { }

ETypeType CType::GetType()
{
    return type;
}

bool CType::GetRef()
{
    return ref;
}

void CType::SetRef(bool _ref)
{
    ref = _ref;
}

bool CType::IsIdent()
{
    return isIdent;
}

void CType::SetIsIdent(bool _isIdent)
{
    isIdent = _isIdent;
}

CType::~CType() { }