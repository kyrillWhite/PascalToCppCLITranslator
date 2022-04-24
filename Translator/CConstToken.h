#pragma once
#include "CToken.h"
#include "CVariant.h"
#include "CIntVariant.h"
#include "CRealVariant.h"
#include "CStringVariant.h"
#include "CBooleanVariant.h"

class CConstToken : public CToken
{
private:
    std::unique_ptr<CVariant> variant;

public:
    CConstToken(std::unique_ptr<CVariant> _variant);
    CVariant* GetVariant();
    std::string ToString() override;
};