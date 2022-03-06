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

    void Program();                         // Программа
    void Block();                           // Блок
    void TypesSection();                    // Раздел типов
    void TypeDefinition();                  // Определение типа
    void Type();                            // Тип
    void VariablesSection();                // Раздел переменных
    void SameTypeVars();                    // Описание однотипных переменных
    void ProceduresSection();               // Раздел процедур
    void ProcedureDefinition();             // Описание процедуры
    void ProcedureHeader();                 // Заголовок процедуры
    void ParametersGroup();                 // Группа параметров
    void FormalParametersSection();         // Раздел формальных параметров
    void OperatorsSection();                // Раздел операторов
    void CompOperator();                    // Составной оператор
    void Operator();                        // Оператор
    void SimpleOperator();                  // Простой оператор
    void AssignOperator();                  // Оператор присваивания
    void Expression();                      // Выражение
    void SimpleExpression();                // Простое выражение
    void Summand();                         // Слагаемое
    void Multiplier();                      // Множитель
    void ProcedureOperator();               // Оператор процедуры
    void ActualParameter();                 // Фактический параметр
    void ComplexOperator();                 // Сложный оператор
    void ConditionalOperator();             // Условный оператор
    void LoopWithPrecondition();            // Цикл c предусловием 
    void LoopWithPostcondition();           // Цикл c постусловием 
    void LoopWithParameter();               // Цикл c параметром 
    void Dirrection();                      // Направление

public:
    CParser(std::unique_ptr<CLexer> _lexer);
    CLexer* GetLexerPtr();
    void Evaluate();
};