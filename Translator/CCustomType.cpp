#include "CCustomType.h"

CCustomType::CCustomType(std::shared_ptr<CType> _sourceType, bool _isType) : 
    CType(ttCustom), sourceType(_sourceType), isType(_isType) { }

std::shared_ptr<CType> CCustomType::GetSourceType()
{
    return sourceType;
}

std::shared_ptr<CType> CCustomType::GetOriginType()
{
    auto type = sourceType;
    if (type && type->GetType() == ttCustom) {
        type = std::dynamic_pointer_cast<CCustomType>(type)->GetOriginType();
    }
    return type;
}

bool CCustomType::IsType()
{
    return isType;
}
