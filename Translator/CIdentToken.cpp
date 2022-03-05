#include "CIdentToken.h"

CIdentToken::CIdentToken(std::string _name) : CToken(ttIdent), name(_name) { }

std::string CIdentToken::ToString()
{
    return name;
}
