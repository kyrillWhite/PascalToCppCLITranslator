#pragma once
#include "CType.h"
#include <vector>
#include <memory>

class CProcedureType : public CType
{
private:
    std::vector<std::shared_ptr<CType>> attributes;

public:
    CProcedureType();
    void SetAttributes(std::vector<std::shared_ptr<CType>> _attributes);
    std::vector<std::shared_ptr<CType>> GetAttributes();
};