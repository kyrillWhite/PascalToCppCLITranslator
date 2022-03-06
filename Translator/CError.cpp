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
        return "����������� ������";
    case e050:
        return "������ � ���������";
    case e075:
        return "������ � ���������� ���������";
    case e077:
        return "��������� ��������� �� �������";
    case e086:
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
