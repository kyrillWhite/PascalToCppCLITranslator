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
        return "������ ���� ���";
    case e003:
        return "������ ���� ��������� ����� PROGRAM";
    case e004:
        return "������ ���� ������ \')\'";
    case e005:
        return "������ ���� ������ \':\'";
    case e006:
        return "����������� ������";
    case e013:
        return "������ ���� ����� END";
    case e014:
        return "������ ���� ������ \';\'";
    case e016:
        return "������ ���� ������ \'=\'";
    case e017:
        return "������ ���� ����� BEGIN";
    case e021:
        return "��������� ���������";
    case e050:
        return "������ � ���������";
    case e051:
        return "������ ���� ������ \':=\'";
    case e052:
        return "������ ���� ����� THEN";
    case e053:
        return "������ ���� ����� UNTIL";
    case e054:
        return "������ ���� ����� DO";
    case e055:
        return "������ ���� ����� TO ��� DOWNTO";
    case e061:
        return "������ ���� ������ \'.\'";
    case e075:
        return "������ � ���������� ���������";
    case e077:
        return "��������� ��������� �� �������";
    case e086:
        return "����������� �� ������";
    case e100:
        return "������������� ����� �� ������������� ��������";
    case e101:
        return "��� �������� ��������";
    case e104:
        return "��� �� �������";
    case e135:
        return "��� �������� ������ ���� BOOLEAN";
    case e143:
        return "������������ ��� ��������� �����";
    case e182:
        return "���� �� ���������";
    case e184:
        return "������� ����� ���� �� ����� ����� ����";
    case e186:
        return "�������������� ����� ��� �������� ���������";
    case e198:
        return "�������������� ���������� ���������� ���������";
    case e199:
        return "�������������� ����� ���������� ���������";
    case e201:
        return "������ � ������������ ���������: ������ ���� �����";
    case e203:
        return "����� ��������� ��������� ������";
    case e206:
        return "������� ��������� ������������ ���������";
    case e207:
        return "������� ������� ������������ ���������";
    case e210:
        return "�������� AND, NOT, OR ������ ���� ��������";
    case e211:
        return "������������ ���� ��������� �������� + ��� �";
    case e212:
        return "�������� DIV � MOD ������ ���� ������";
    case e213:
        return "������������ ���� ��������� �������� *";
    case e214:
        return "������������ ���� ��������� �������� /";
    case e215:
        return "��������, ������������ �� ������, ������ �������� ����������";
    default:
        return "";
    }
}
