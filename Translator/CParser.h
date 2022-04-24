#pragma once
#include "CLexer.h"
#include "CScopes.h"
#include "CGenerator.h"

class CParser
{
private:
    std::unique_ptr<CToken> curToken;
    std::unique_ptr<CScopes> scopes;
    std::shared_ptr<CLexer> lexer;
    std::shared_ptr<CGenerator> generator;
    std::map<std::string, ETypeType> typeNames;

    bool isNeutralizeErrors;

    void GetNextToken();
    void Accept(ETokenType tType);
    void Accept(EKeyWordType kwType);
    bool Belong(std::vector<std::shared_ptr<CToken>> tokens);
    void SkipTo(std::vector<std::shared_ptr<CToken>> followers);

    void AddIdent(std::string name, std::shared_ptr<CType> type);
    std::shared_ptr<CType> DeclaringCheckIdent();
    std::string GetIdentName();
    std::shared_ptr<CType> GetTypeByName(std::string name);
    std::shared_ptr<CType> GetConstantType();
    void AcceptProcedureCallTypes(std::shared_ptr<CType> proc, std::vector<std::shared_ptr<CType>> attributes);
    bool IsVariableType(std::shared_ptr<CType> type);
    bool IsEqualTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2);
    bool IsScalarTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2);
    bool IsScalarType(std::shared_ptr<CType> type);
    bool IsBooleanTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2);
    bool IsBooleanType(std::shared_ptr<CType> type);
    bool IsIntegerTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2);
    bool IsIntegerAndReal(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2);
    bool IsIntegerType(std::shared_ptr<CType> type);
    bool IsStringTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2);

    bool IsRelOper();
    bool IsSign();
    bool IsAddOper();
    bool IsMultOper();

    bool EqualKeyWord(EKeyWordType kwType);

    void Program(std::vector<std::shared_ptr<CToken>> followers = {});                   // ���������
    void Block(std::string callingName, 
        std::vector<std::shared_ptr<CToken>> followers = {});                     // ����
    void TypesSection(std::vector<std::shared_ptr<CToken>> followers = {});              // ������ �����
    void TypeDefinition(std::vector<std::shared_ptr<CToken>> followers = {});            // ����������� ����
    void Type(std::vector<std::shared_ptr<CToken>> followers = {});                      // ���
    void VariablesSection(std::vector<std::shared_ptr<CToken>> followers = {});          // ������ ����������
    void SameTypeVars(std::vector<std::shared_ptr<CToken>> followers = {});              // �������� ���������� ����������
    void ProceduresSection(std::vector<std::shared_ptr<CToken>> followers = {});         // ������ ��������
    void ProcedureDefinition(std::vector<std::shared_ptr<CToken>> followers = {});       // �������� ���������
    std::vector<std::shared_ptr<CType>> 
        ProcedureHeader(std::vector<std::shared_ptr<CToken>> followers = {});            // ��������� ���������
    std::vector<std::shared_ptr<CType>> 
        ParametersGroup(bool isRef, std::vector<std::shared_ptr<CToken>> followers = {});// ������ ����������
    std::vector<std::shared_ptr<CType>> 
        FormalParametersSection(std::vector<std::shared_ptr<CToken>> followers = {});    // ������ ���������� ����������
    void OperatorsSection(std::vector<std::shared_ptr<CToken>> followers = {});          // ������ ����������
    void CompOperator(std::vector<std::shared_ptr<CToken>> followers = {});              // ��������� ��������
    void Operator(std::vector<std::shared_ptr<CToken>> followers = {});                  // ��������
    void SimpleOperator(std::vector<std::shared_ptr<CToken>> followers = {});            // ������� ��������
    std::shared_ptr<CType>  
        AssignOperator(std::vector<std::shared_ptr<CToken>> followers = {});             // �������� ������������
    std::shared_ptr<CType> 
        Expression(std::vector<std::shared_ptr<CToken>> followers = {});                 // ���������
    std::shared_ptr<CType> 
        SimpleExpression(std::vector<std::shared_ptr<CToken>> followers = {});           // ������� ���������
    std::shared_ptr<CType> 
        Summand(std::vector<std::shared_ptr<CToken>> followers = {});                    // ���������
    std::shared_ptr<CType> 
        Multiplier(std::vector<std::shared_ptr<CToken>> followers = {});                 // ���������
    std::vector<std::shared_ptr<CType>> 
        ProcedureOperator(std::vector<std::shared_ptr<CToken>> followers = {});          // �������� ���������
    std::shared_ptr<CType> 
        ActualParameter(std::vector<std::shared_ptr<CToken>> followers = {});            // ����������� ��������
    void ComplexOperator(std::vector<std::shared_ptr<CToken>> followers = {});           // ������� ��������
    void ConditionalOperator(std::vector<std::shared_ptr<CToken>> followers = {});       // �������� ��������
    void LoopWithPrecondition(std::vector<std::shared_ptr<CToken>> followers = {});      // ���� c ������������ 
    void LoopWithPostcondition(std::vector<std::shared_ptr<CToken>> followers = {});     // ���� c ������������ 
    void LoopWithParameter(std::vector<std::shared_ptr<CToken>> followers = {});         // ���� c ���������� 
    bool Dirrection(std::vector<std::shared_ptr<CToken>> followers = {});                // �����������

public:
    CParser(std::shared_ptr<CLexer> _lexer, std::shared_ptr<CGenerator> _generator, bool _isNeutralizeErrors = true);
    std::shared_ptr<CLexer> GetLexerPtr();
    void Evaluate();
};