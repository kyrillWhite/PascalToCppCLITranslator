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
    case e6:
        return "����������� ������";
    case e75:
        return "������ � ���������� ���������";
    case e77:
        return "��������� ��������� �� �������";
    case e86:
        return "����������� �� ������";
    case e201:
        return "������ � ������������ ���������: ������ ���� �����";
    case e203:
        return "����� ��������� ��������� ������";
    case e206:
        return "������� ��������� ������������ ���������";
    case e207:
        return "������� ������� ������������ ���������";
    default:
        return "";
    }
}
