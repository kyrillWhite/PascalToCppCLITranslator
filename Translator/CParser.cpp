#include "CParser.h"

void CParser::GetNextToken()
{
    curToken = lexer->GetNextToken();
}

void CParser::Accept(ETokenType tType)
{
    if (curToken->GetType() != tType) {
        switch (tType) {
        case ttIdent:
            lexer->GetIOPtr()->AddError(e002);
        }
    }
    GetNextToken();
}

void CParser::Accept(EKeyWordType kwType)
{
    if (curToken->GetType() != ttKeyword ||
        dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType() != kwType) {
        switch (kwType) {
        case programSy:
            lexer->GetIOPtr()->AddError(e003);
        case rightparSy:
            lexer->GetIOPtr()->AddError(e004);
        case colonSy:
            lexer->GetIOPtr()->AddError(e005);
        case endSy:
            lexer->GetIOPtr()->AddError(e013);
        case semicolonSy:
            lexer->GetIOPtr()->AddError(e014);
        case equalSy:
            lexer->GetIOPtr()->AddError(e016);
        case beginSy:
            lexer->GetIOPtr()->AddError(e017);
        case assignSy:
            lexer->GetIOPtr()->AddError(e051);
        case thenSy:
            lexer->GetIOPtr()->AddError(e052);
        case untilSy:
            lexer->GetIOPtr()->AddError(e053);
        case doSy:
            lexer->GetIOPtr()->AddError(e054);
        default:
            std::cout << "?";
        }
    }
    GetNextToken();
}

bool CParser::IsRelOper()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    return kwType == equalSy || kwType == laterSy || kwType == greaterSy ||
        kwType == laterequalSy || kwType == greaterequalSy || kwType == latergreaterSy;
}

bool CParser::IsSign()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    return kwType == plusSy || kwType == minusSy;
}

bool CParser::IsAddOper()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    return kwType == plusSy || kwType == minusSy || kwType == orSy;
}

bool CParser::IsMultOper()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    return kwType == starSy || kwType == slashSy || 
        kwType == divSy || kwType == modSy || kwType == andSy;
}

bool CParser::EqualKeyWord(EKeyWordType kwType)
{
    return curToken->GetType() == ttKeyword &&
        dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType() == kwType;
}

void CParser::Program()
{
    Accept(programSy);
    Accept(ttIdent);
    Accept(semicolonSy);
    Block();
}

void CParser::Block()
{
    TypesSection();
    VariablesSection();
    ProceduresSection();
    OperatorsSection();
}

void CParser::TypesSection()
{
    if (EqualKeyWord(typeSy)) {
        GetNextToken();
        TypeDefinition();
        Accept(semicolonSy);
        while (curToken->GetType() == ttIdent) {
            TypeDefinition();
            Accept(semicolonSy);
        }
    }
}

void CParser::TypeDefinition()
{
    Accept(ttIdent);
    Accept(equalSy);
    Type();
}

void CParser::Type()
{
    Accept(ttIdent);
}

void CParser::VariablesSection()
{
    if (EqualKeyWord(varSy)) {
        GetNextToken();
        SameTypeVars();
        Accept(semicolonSy);
        while (curToken->GetType() == ttIdent) {
            SameTypeVars();
            Accept(semicolonSy);
        }
    }
}

void CParser::SameTypeVars()
{
    Accept(ttIdent);
    while (EqualKeyWord(commaSy)) {
        GetNextToken();
        Accept(ttIdent);
    }
    Accept(colonSy);
    Type();
}

void CParser::ProceduresSection()
{
    while (EqualKeyWord(procedureSy)) {
        ProcedureDefinition();
        Accept(semicolonSy);
    }
}

void CParser::ProcedureDefinition()
{
    ProcedureHeader();
    Block();
}

void CParser::ProcedureHeader()
{
    GetNextToken();
    Accept(ttIdent);
    if (EqualKeyWord(leftparSy)) {
        Accept(leftparSy);
        FormalParametersSection();
        while (EqualKeyWord(semicolonSy)) {
            Accept(semicolonSy);        
            FormalParametersSection();
        }
        Accept(rightparSy);
    }
    Accept(semicolonSy);
}

void CParser::FormalParametersSection()
{
    if (EqualKeyWord(varSy)) {
        GetNextToken();
        ParametersGroup();
    }
    else if (EqualKeyWord(procedureSy)) {
        GetNextToken();
        Accept(ttIdent);
        while (EqualKeyWord(commaSy)) {
            GetNextToken();
            Accept(ttIdent);
        }
    }
    else {
        ParametersGroup();
    }
}

void CParser::ParametersGroup()
{
    Accept(ttIdent);
    while (EqualKeyWord(commaSy)) {
        GetNextToken();
        Accept(ttIdent);
    }
    Accept(colonSy);
    Accept(ttIdent);
}

void CParser::OperatorsSection()
{
    CompOperator();
}

void CParser::CompOperator()
{
    Accept(beginSy);
    Operator();
    while (EqualKeyWord(semicolonSy)) {
        GetNextToken();
        Operator();
    }
    Accept(endSy);
}

void CParser::Operator()
{
    if (curToken->GetType() == ttIdent) {
        SimpleOperator();
    }
    else if (curToken->GetType() == ttKeyword) {
        auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
        if (kwType == beginSy || kwType == ifSy || kwType == whileSy ||
            kwType == repeatSy || kwType == forSy) {
            ComplexOperator();
        }
    }
}

void CParser::SimpleOperator()
{
    Accept(ttIdent);
    if (EqualKeyWord(assignSy)) {
        AssignOperator();
    }
    else {
        ProcedureOperator();
    }
}

void CParser::AssignOperator()
{
    Accept(assignSy);
    Expression();
}

void CParser::Expression()
{
    SimpleExpression();
    if (IsRelOper()) {
        GetNextToken();
        SimpleExpression();
    }
}

void CParser::SimpleExpression()
{
    if (IsSign()) {
        GetNextToken();
    }
    Summand();
    while (IsAddOper()) {
        GetNextToken();
        Summand();
    }
}

void CParser::Summand()
{
    Multiplier();
    while (IsMultOper()) {
        GetNextToken();
        Multiplier();
    }
}

void CParser::Multiplier()
{
    if (curToken->GetType() == ttIdent || curToken->GetType() == ttConst) {
        GetNextToken();
    }
    else if (EqualKeyWord(leftparSy)) {
        GetNextToken();
        Expression();
        Accept(rightparSy);
    }
    else if (EqualKeyWord(notSy)) {
        GetNextToken();
        Multiplier();
    }
    else {
        lexer->GetIOPtr()->AddError(e021);
    }
}

void CParser::ProcedureOperator()
{
    if (EqualKeyWord(leftparSy)) {
        GetNextToken();
        ActualParameter();
        while (EqualKeyWord(commaSy)) {
            GetNextToken();
            ActualParameter();
        }
    }
}

void CParser::ActualParameter()
{
    Expression();
}

void CParser::ComplexOperator()
{
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    switch (kwType) {
    case beginSy:
        CompOperator();
        break;
    case ifSy:
        ConditionalOperator();
        break;
    case whileSy:
        LoopWithPrecondition();
        break;
    case repeatSy:
        LoopWithPostcondition();
        break;
    case forSy:
        LoopWithParameter();
        break;
    }
}

void CParser::ConditionalOperator()
{
    GetNextToken();
    Expression();
    Accept(thenSy);
    Operator();
    if (EqualKeyWord(elseSy)) {
        GetNextToken();
        Operator();
    }
}

void CParser::LoopWithPrecondition()
{
    GetNextToken();
    Expression();
    Accept(doSy);
    Operator();
}

void CParser::LoopWithPostcondition()
{
    GetNextToken();
    Operator();
    while (EqualKeyWord(semicolonSy)) {
        GetNextToken();
        Operator();
    }
    Accept(untilSy);
    Expression();
}

void CParser::LoopWithParameter()
{
    GetNextToken();
    Accept(ttIdent);
    Accept(assignSy);
    Expression();
    Dirrection();
    Expression();
    Accept(doSy);
    Operator();
}

void CParser::Dirrection()
{
    if (EqualKeyWord(toSy) || EqualKeyWord(downtoSy)) {
        GetNextToken();
        return;
    }
    lexer->GetIOPtr()->AddError(e055);
}

CParser::CParser(std::unique_ptr<CLexer> _lexer) : lexer(move(_lexer)) { }

CLexer* CParser::GetLexerPtr()
{
    return lexer.get();
}

void CParser::Evaluate()
{
    GetNextToken();
    Program();
}