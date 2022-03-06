#include "CConstToken.h"

using namespace std;

CConstToken::CConstToken(unique_ptr<CVariant> _variant) :
    CToken(ttConst), variant(move(_variant)) { }

CVariant* CConstToken::GetVariant()
{
    return variant.get();
}

std::string CConstToken::ToString()
{
    return variant->ToString();
}
