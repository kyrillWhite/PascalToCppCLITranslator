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
    case e100:
        return "Использование имени не соответствует описанию";
    case e101:
        return "Имя описанно повторно";
    case e104:
        return "Имя не описано";
    case e135:
        return "Тип операнда должен быть BOOLEAN";
    case e143:
        return "Недопустимый тип параметра цикла";
    case e182:
        return "Типы не совместны";
    case e184:
        return "Элемент этого типа не может иметь знак";
    case e186:
        return "Несоответствие типов для операции отношения";
    case e198:
        return "Несоответствие количества параметров процедуры";
    case e199:
        return "Несоответствие типов параметров процедуры";
    case e201:
        return "Ошибка в вещественной константе: должна идти цифра";
    case e203:
        return "Целая константа превышает предел";
    case e206:
        return "Слишком маленькая вещественная константа";
    case e207:
        return "Слишком большая вещественная константа";
    case e210:
        return "Операнды AND, NOT, OR должны быть булевыми";
    case e211:
        return "Недопустимые типы операндов операции + или —";
    case e212:
        return "Операнды DIV и MOD должны быть целыми";
    case e213:
        return "Недопустимые типы операндов операции *";
    case e214:
        return "Недопустимые типы операндов операции /";
    case e215:
        return "Параметр, передаваемый по ссылке, должен являться переменной";
    default:
        return "";
    }
}
