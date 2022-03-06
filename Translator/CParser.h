#pragma once

#include "CLexer.h"

class CParser
{
private:
    std::unique_ptr<CToken> curToken;
    std::unique_ptr<CLexer> lexer;

    void GetNextToken();
    void Accept(ETokenType tType);
    void Accept(EKeyWordType kwType);

    bool IsRelOper();
    bool IsSign();
    bool IsAddOper();
    bool IsMultOper();

    bool EqualKeyWord(EKeyWordType kwType);

    void Program();                         // ���������
    void Block();                           // ����
    void TypesSection();                    // ������ �����
    void TypeDefinition();                  // ����������� ����
    void Type();                            // ���
    void VariablesSection();                // ������ ����������
    void SameTypeVars();                    // �������� ���������� ����������
    void ProceduresSection();               // ������ ��������
    void ProcedureDefinition();             // �������� ���������
    void ProcedureHeader();                 // ��������� ���������
    void ParametersGroup();                 // ������ ����������
    void FormalParametersSection();         // ������ ���������� ����������
    void OperatorsSection();                // ������ ����������
    void CompOperator();                    // ��������� ��������
    void Operator();                        // ��������
    void SimpleOperator();                  // ������� ��������
    void AssignOperator();                  // �������� ������������
    void Expression();                      // ���������
    void SimpleExpression();                // ������� ���������
    void Summand();                         // ���������
    void Multiplier();                      // ���������
    void ProcedureOperator();               // �������� ���������
    void ActualParameter();                 // ����������� ��������
    void ComplexOperator();                 // ������� ��������
    void ConditionalOperator();             // �������� ��������
    void LoopWithPrecondition();            // ���� c ������������ 
    void LoopWithPostcondition();           // ���� c ������������ 
    void LoopWithParameter();               // ���� c ���������� 
    void Dirrection();                      // �����������

public:
    CParser(std::unique_ptr<CLexer> _lexer);
    CLexer* GetLexerPtr();
    void Evaluate();
};