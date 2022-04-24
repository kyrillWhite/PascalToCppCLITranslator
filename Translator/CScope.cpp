#include "CScope.h"

CScope::CScope(std::shared_ptr<CScope> _parent)
{
    parent = _parent;
}

std::shared_ptr<CScope> CScope::GetScopeParent()
{
    return parent;
}

void CScope::SetScopeParent(std::shared_ptr<CScope> _parent)
{
    parent = _parent;
}

std::shared_ptr<CType> CScope::AddIdent(std::string name, std::shared_ptr<CType> ident)
{
    if (LookupIdent(name)) {
        return nullptr;
    }
    idents[name] = ident;
    return ident;
}

std::shared_ptr<CType> CScope::LookupIdent(std::string name)
{
    if (idents[name]) {
        return idents[name];
    }
    if (parent) {
        return parent->LookupIdent(name);
    }
    return nullptr;
}
