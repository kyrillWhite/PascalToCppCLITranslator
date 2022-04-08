#pragma once

#include "CLexer.h"

class CParser
{
private:
    std::unique_ptr<CToken> curToken;
    std::shared_ptr<CLexer> lexer;

    void GetNextToken();
    void Accept(ETokenType tType);
    void Accept(EKeyWordType kwType);
    bool Belong(std::vector<std::shared_ptr<CToken>> tokens);
    void SkipTo(std::vector<std::shared_ptr<CToken>> followers);

    bool IsRelOper();
    bool IsSign();
    bool IsAddOper();
    bool IsMultOper();

    bool EqualKeyWord(EKeyWordType kwType);

    void Program(std::vector<std::shared_ptr<CToken>> followers = {});                   // ���������
    void Block(std::vector<std::shared_ptr<CToken>> followers = {});                     // ����
    void TypesSection(std::vector<std::shared_ptr<CToken>> followers = {});              // ������ �����
    void TypeDefinition(std::vector<std::shared_ptr<CToken>> followers = {});            // ����������� ����
    void Type(std::vector<std::shared_ptr<CToken>> followers = {});                      // ���
    void VariablesSection(std::vector<std::shared_ptr<CToken>> followers = {});          // ������ ����������
    void SameTypeVars(std::vector<std::shared_ptr<CToken>> followers = {});              // �������� ���������� ����������
    void ProceduresSection(std::vector<std::shared_ptr<CToken>> followers = {});         // ������ ��������
    void ProcedureDefinition(std::vector<std::shared_ptr<CToken>> followers = {});       // �������� ���������
    void ProcedureHeader(std::vector<std::shared_ptr<CToken>> followers = {});           // ��������� ���������
    void ParametersGroup(std::vector<std::shared_ptr<CToken>> followers = {});           // ������ ����������
    void FormalParametersSection(std::vector<std::shared_ptr<CToken>> followers = {});   // ������ ���������� ����������
    void OperatorsSection(std::vector<std::shared_ptr<CToken>> followers = {});          // ������ ����������
    void CompOperator(std::vector<std::shared_ptr<CToken>> followers = {});              // ��������� ��������
    void Operator(std::vector<std::shared_ptr<CToken>> followers = {});                  // ��������
    void SimpleOperator(std::vector<std::shared_ptr<CToken>> followers = {});            // ������� ��������
    void AssignOperator(std::vector<std::shared_ptr<CToken>> followers = {});            // �������� ������������
    void Expression(std::vector<std::shared_ptr<CToken>> followers = {});                // ���������
    void SimpleExpression(std::vector<std::shared_ptr<CToken>> followers = {});          // ������� ���������
    void Summand(std::vector<std::shared_ptr<CToken>> followers = {});                   // ���������
    void Multiplier(std::vector<std::shared_ptr<CToken>> followers = {});                // ���������
    void ProcedureOperator(std::vector<std::shared_ptr<CToken>> followers = {});         // �������� ���������
    void ActualParameter(std::vector<std::shared_ptr<CToken>> followers = {});           // ����������� ��������
    void ComplexOperator(std::vector<std::shared_ptr<CToken>> followers = {});           // ������� ��������
    void ConditionalOperator(std::vector<std::shared_ptr<CToken>> followers = {});       // �������� ��������
    void LoopWithPrecondition(std::vector<std::shared_ptr<CToken>> followers = {});      // ���� c ������������ 
    void LoopWithPostcondition(std::vector<std::shared_ptr<CToken>> followers = {});     // ���� c ������������ 
    void LoopWithParameter(std::vector<std::shared_ptr<CToken>> followers = {});         // ���� c ���������� 
    void Dirrection(std::vector<std::shared_ptr<CToken>> followers = {});                // �����������

public:
    CParser(std::shared_ptr<CLexer> _lexer);
    std::shared_ptr<CLexer> GetLexerPtr();
    void Evaluate();
};