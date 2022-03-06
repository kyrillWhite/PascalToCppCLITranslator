#include "CError.h"

using namespace std;

CError::CError(int _lineNumber, int _symbolNumber, EErrorType _eType) :
    lineNumber(_lineNumber), symbolNumber(_symbolNumber), eType(_eType) { }

int CError::GetLineNum()
{
    return lineNumber;
}

int CError::GetSymbolNum()
{
    return symbolNumber;
}

string CError::GetText()
{
    switch (eType) {
    case e006:
        return "Запрещенный символ";
    case e050:
        return "Ошибка в константе";
    case e075:
        return "Ошибка в символьной константе";
    case e077:
        return "Строковая константа не закрыта";
    case e086:
        return "Комментарий не закрыт";
    case e201:
        return "Ошибка в вещественной константе: должна идти цифра";
    case e203:
        return "Целая константа превышает предел";
    case e206:
        return "Слишком маленькая вещественная константа";
    case e207:
        return "Слишком большая вещественная константа";
    default:
        return "";
    }
}
