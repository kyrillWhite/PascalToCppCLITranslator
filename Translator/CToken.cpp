#include "CToken.h"

CToken::CToken(ETokenType _tType) : tType(_tType) { }

ETokenType CToken::GetType()
{
    return tType;
}

CToken::~CToken() { }