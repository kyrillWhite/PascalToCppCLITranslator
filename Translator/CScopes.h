#pragma once
#include "CScope.h"

class CScopes
{
private:
    std::shared_ptr<CScope> current;

public:
    CScopes();

    std::shared_ptr<CScope> GetCurrent();
    std::shared_ptr<CScope> Pop();
    void Push(std::shared_ptr<CScope> scope);
};