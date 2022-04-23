#pragma once
#include "CType.h"
#include <memory>

class CCustomType : public CType
{
    std::shared_ptr<CType> sourceType;
    bool isType;
public:
    CCustomType(std::shared_ptr<CType> _sourceType, bool _isType);

    std::shared_ptr<CType> GetSourceType();
    std::shared_ptr<CType> GetOriginType();
    bool IsType();
};