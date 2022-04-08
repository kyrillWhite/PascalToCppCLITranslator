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

    void Program(std::vector<std::shared_ptr<CToken>> followers = {});                   // Программа
    void Block(std::vector<std::shared_ptr<CToken>> followers = {});                     // Блок
    void TypesSection(std::vector<std::shared_ptr<CToken>> followers = {});              // Раздел типов
    void TypeDefinition(std::vector<std::shared_ptr<CToken>> followers = {});            // Определение типа
    void Type(std::vector<std::shared_ptr<CToken>> followers = {});                      // Тип
    void VariablesSection(std::vector<std::shared_ptr<CToken>> followers = {});          // Раздел переменных
    void SameTypeVars(std::vector<std::shared_ptr<CToken>> followers = {});              // Описание однотипных переменных
    void ProceduresSection(std::vector<std::shared_ptr<CToken>> followers = {});         // Раздел процедур
    void ProcedureDefinition(std::vector<std::shared_ptr<CToken>> followers = {});       // Описание процедуры
    void ProcedureHeader(std::vector<std::shared_ptr<CToken>> followers = {});           // Заголовок процедуры
    void ParametersGroup(std::vector<std::shared_ptr<CToken>> followers = {});           // Группа параметров
    void FormalParametersSection(std::vector<std::shared_ptr<CToken>> followers = {});   // Раздел формальных параметров
    void OperatorsSection(std::vector<std::shared_ptr<CToken>> followers = {});          // Раздел операторов
    void CompOperator(std::vector<std::shared_ptr<CToken>> followers = {});              // Составной оператор
    void Operator(std::vector<std::shared_ptr<CToken>> followers = {});                  // Оператор
    void SimpleOperator(std::vector<std::shared_ptr<CToken>> followers = {});            // Простой оператор
    void AssignOperator(std::vector<std::shared_ptr<CToken>> followers = {});            // Оператор присваивания
    void Expression(std::vector<std::shared_ptr<CToken>> followers = {});                // Выражение
    void SimpleExpression(std::vector<std::shared_ptr<CToken>> followers = {});          // Простое выражение
    void Summand(std::vector<std::shared_ptr<CToken>> followers = {});                   // Слагаемое
    void Multiplier(std::vector<std::shared_ptr<CToken>> followers = {});                // Множитель
    void ProcedureOperator(std::vector<std::shared_ptr<CToken>> followers = {});         // Оператор процедуры
    void ActualParameter(std::vector<std::shared_ptr<CToken>> followers = {});           // Фактический параметр
    void ComplexOperator(std::vector<std::shared_ptr<CToken>> followers = {});           // Сложный оператор
    void ConditionalOperator(std::vector<std::shared_ptr<CToken>> followers = {});       // Условный оператор
    void LoopWithPrecondition(std::vector<std::shared_ptr<CToken>> followers = {});      // Цикл c предусловием 
    void LoopWithPostcondition(std::vector<std::shared_ptr<CToken>> followers = {});     // Цикл c постусловием 
    void LoopWithParameter(std::vector<std::shared_ptr<CToken>> followers = {});         // Цикл c параметром 
    void Dirrection(std::vector<std::shared_ptr<CToken>> followers = {});                // Направление

public:
    CParser(std::shared_ptr<CLexer> _lexer);
    std::shared_ptr<CLexer> GetLexerPtr();
    void Evaluate();
};