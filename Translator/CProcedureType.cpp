#include "CProcedureType.h"


CProcedureType::CProcedureType() : CType(ttProcedure) { }

void CProcedureType::SetAttributes(std::vector<std::shared_ptr<CType>> _attributes)
{ 
    attributes = _attributes;
}

std::vector<std::shared_ptr<CType>> CProcedureType::GetAttributes()
{
    return attributes;
}
