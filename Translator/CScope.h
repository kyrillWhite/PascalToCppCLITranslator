#pragma once
#include "CBooleanType.h"
#include "CCustomType.h"
#include "CIntType.h"
#include "CNameType.h"
#include "CRealType.h"
#include "CStringType.h"
#include "CProcedureType.h"
#include <memory>
#include <string>
#include <map>

class CScope
{
private:
    std::shared_ptr<CScope> parent;
    std::map<std::string, std::shared_ptr<CType>> idents;

public:
    CScope(std::shared_ptr<CScope> _parent = nullptr);

    std::shared_ptr<CScope> GetScopeParent();
    void SetScopeParent(std::shared_ptr<CScope> _parent);

    std::shared_ptr<CType> AddIdent(std::string name, std::shared_ptr<CType> ident);
    std::shared_ptr<CType> LookupIdent(std::string name);
};