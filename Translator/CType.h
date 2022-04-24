#pragma once
#include "ETypeType.h"

class CType
{
private:
    ETypeType type;
    bool ref;
    bool isIdent;

public:
    CType(ETypeType _type);
    ETypeType GetType();
    bool GetRef();
    void SetRef(bool _ref);
    bool IsIdent();
    void SetIsIdent(bool _isIdent);

    virtual ~CType();
};