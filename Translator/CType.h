#pragma once
#include "ETypeType.h"

class CType
{
private:
    ETypeType type;

public:
    CType(ETypeType _type);
    ETypeType GetType();

    virtual ~CType();
};