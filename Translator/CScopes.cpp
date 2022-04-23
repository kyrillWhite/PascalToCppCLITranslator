#include "CScopes.h"

CScopes::CScopes()
{
    current = nullptr;
}

std::shared_ptr<CScope> CScopes::GetCurrent()
{
    return current;
}

std::shared_ptr<CScope> CScopes::Pop()
{
    std::shared_ptr<CScope> curScope = current;
    if (current) {
        current = current->GetScopeParent();
    }
    return curScope;
}

void CScopes::Push(std::shared_ptr<CScope> scope)
{
    scope->SetScopeParent(current);
    current = scope;
}