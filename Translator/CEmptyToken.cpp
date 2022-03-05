#include "CEmptyToken.h"

CEmptyToken::CEmptyToken() : CToken(ttEmpty) { }

std::string CEmptyToken::ToString()
{
    return "";
}
