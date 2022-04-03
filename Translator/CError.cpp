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
    case e002:
        return "Должно идти имя";
    case e003:
        return "Должно быть служебное слово PROGRAM";
    case e004:
        return "Должен идти символ \')\'";
    case e005:
        return "Должен идти символ \':\'";
    case e006:
        return "Запрещенный символ";
    case e013:
        return "Должно идти слово END";
    case e014:
        return "Должен идти символ \';\'";
    case e016:
        return "Должен идти символ \'=\'";
    case e017:
        return "Должно идти слово BEGIN";
    case e021:
        return "Требуется выражение";
    case e050:
        return "Ошибка в константе";
    case e051:
        return "Должен идти символ \':=\'";
    case e052:
        return "Должно идти слово THEN";
    case e053:
        return "Должно идти слово UNTIL";
    case e054:
        return "Должно идти слово DO";
    case e055:
        return "Должно идти слово TO или DOWNTO";
    case e061:
        return "Должен идти символ \'.\'";
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
