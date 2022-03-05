#include "CKeyWordToken.h"

CKeyWordToken::CKeyWordToken(EKeyWordType _keyWordType) :
    CToken(ttKeyword), keyWordType(_keyWordType) { }

EKeyWordType CKeyWordToken::GetKeyWordType()
{
    return keyWordType;
}

std::string CKeyWordToken::ToString()
{
    switch (keyWordType) {
    case programSy:
        return "program";
    case varSy:
        return "var";
    case booleanSy:
        return "boolean";
    case integerSy:
        return "integer";
    case realSy:
        return "real";
    case stringSy:
        return "string";
    case procedureSy:
        return "procedure";
    case orSy:
        return "or";
    case andSy:
        return "and";
    case divSy:
        return "div";
    case modSy:
        return "mod";
    case beginSy:
        return "begin";
    case endSy:
        return "end";
    case ifSy:
        return "if";
    case thenSy:
        return "then";
    case elseSy:
        return "else";
    case whileSy:
        return "while";
    case doSy:
        return "do";
    case repeatSy:
        return "repeat";
    case untilSy:
        return "until";
    case toSy:
        return "to";
    case downtoSy:
        return "downto";
    case starSy:
        return "*";
    case slashSy:
        return "/";
    case equalSy:
        return "=";
    case commaSy:
        return ",";
    case semicolonSy:
        return ";";
    case colonSy:
        return ":";
    case pointSy:
        return ".";
    case arrowSy:
        return "^";
    case leftparSy:
        return "(";
    case rightparSy:
        return ")";
    case lbracketSy:
        return "[";
    case rbracketSy:
        return "]";
    case laterSy:
        return "<";
    case greaterSy:
        return ">";
    case laterequalSy:
        return "<=";
    case greaterequalSy:
        return "=>";
    case latergreaterSy:
        return "<>";
    case plusSy:
        return "+";
    case minusSy:
        return "-";
    case assignSy:
        return ":=";
    case twopointsSy:
        return "..";
    }
}
