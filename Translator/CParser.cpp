#include "CParser.h"

using namespace std;

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
        case pointSy:
            lexer->GetIOPtr()->AddError(e061);
        default:
            cout << "?";
        }
    }
    GetNextToken();
}

bool CParser::Belong(vector<shared_ptr<CToken>> tokens)
{
    auto tType = curToken->GetType();
    auto kwType = tType == ttKeyword ? dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType() : 0;
    auto it = find_if(tokens.begin(), tokens.end(), [tType, kwType](shared_ptr<CToken> token) {
        return token->GetType() == tType && 
            (tType == ttKeyword ? dynamic_cast<CKeyWordToken*>(token.get())->GetKeyWordType() == kwType : true);
    });
    return it != tokens.end();
}

void CParser::SkipTo(vector<shared_ptr<CToken>> followers)
{
    while (!Belong(followers)) {
        GetNextToken();
    }
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

void CParser::Program(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto blockFollowers = followers;
    blockFollowers.push_back(make_shared<CKeyWordToken>(pointSy));
    try {
        Accept(programSy);
        Accept(ttIdent);
        Accept(semicolonSy);
        Block(followers);
        Accept(pointSy);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Block(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto typesSectionFollowers = followers;
    typesSectionFollowers.push_back(make_shared<CKeyWordToken>(varSy));
    typesSectionFollowers.push_back(make_shared<CKeyWordToken>(procedureSy));
    typesSectionFollowers.push_back(make_shared<CKeyWordToken>(beginSy));
    auto variablesSectionFollowers = followers;
    variablesSectionFollowers.push_back(make_shared<CKeyWordToken>(procedureSy));
    variablesSectionFollowers.push_back(make_shared<CKeyWordToken>(beginSy));
    auto proceduresSectionFollowers = followers;
    proceduresSectionFollowers.push_back(make_shared<CKeyWordToken>(beginSy));
    try {
        TypesSection(typesSectionFollowers);
        VariablesSection(variablesSectionFollowers);
        ProceduresSection(proceduresSectionFollowers);
        OperatorsSection(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::TypesSection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto typeDefinitionFollowers = followers;
    typeDefinitionFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    try {
        if (EqualKeyWord(typeSy)) {
            GetNextToken();
            TypeDefinition(typeDefinitionFollowers);
            Accept(semicolonSy);
            while (curToken->GetType() == ttIdent) {
                TypeDefinition(typeDefinitionFollowers);
                Accept(semicolonSy);
            }
        }
        else {
            return;
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::TypeDefinition(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Accept(ttIdent);
        Accept(equalSy);
        Type(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Type(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Accept(ttIdent);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::VariablesSection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto sameTypeVarsFollowers = followers;
    sameTypeVarsFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    try {
        if (EqualKeyWord(varSy)) {
            GetNextToken();
            SameTypeVars(sameTypeVarsFollowers);
            Accept(semicolonSy);
            while (curToken->GetType() == ttIdent) {
                SameTypeVars(sameTypeVarsFollowers);
                Accept(semicolonSy);
            }
        }
        else {
            return;
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::SameTypeVars(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Accept(ttIdent);
        while (EqualKeyWord(commaSy)) {
            GetNextToken();
            Accept(ttIdent);
        }
        Accept(colonSy);
        Type(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ProceduresSection(vector<shared_ptr<CToken>> followers)
{
    if (!EqualKeyWord(procedureSy)) {
        return;
    }
    bool catchedError = false;
    auto procedureDefinitionFollowers = followers;
    procedureDefinitionFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    try {
        while (EqualKeyWord(procedureSy)) {
            ProcedureDefinition(procedureDefinitionFollowers);
            Accept(semicolonSy);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ProcedureDefinition(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        ProcedureHeader(followers);
        Block(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ProcedureHeader(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto formalParametersSectionFollowers = followers;
    formalParametersSectionFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    formalParametersSectionFollowers.push_back(make_shared<CKeyWordToken>(rightparSy));
    try {
        GetNextToken();
        Accept(ttIdent);
        if (EqualKeyWord(leftparSy)) {
            Accept(leftparSy);
            FormalParametersSection(formalParametersSectionFollowers);
            while (EqualKeyWord(semicolonSy)) {
                Accept(semicolonSy);        
                FormalParametersSection(formalParametersSectionFollowers);
            }
            Accept(rightparSy);
        }
        Accept(semicolonSy);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::FormalParametersSection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        if (EqualKeyWord(varSy)) {
            GetNextToken();
            ParametersGroup(followers);
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
            ParametersGroup(followers);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ParametersGroup(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Accept(ttIdent);
        while (EqualKeyWord(commaSy)) {
            GetNextToken();
            Accept(ttIdent);
        }
        Accept(colonSy);
        Accept(ttIdent);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::OperatorsSection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        CompOperator(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::CompOperator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto operatorFollowers = followers;
    operatorFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    operatorFollowers.push_back(make_shared<CKeyWordToken>(endSy));
    try {
        Accept(beginSy);
        Operator(operatorFollowers);
        while (EqualKeyWord(semicolonSy)) {
            GetNextToken();
            Operator(operatorFollowers);
        }
        Accept(endSy);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Operator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        if (curToken->GetType() == ttIdent) {
            SimpleOperator(followers);
        }
        else if (curToken->GetType() == ttKeyword) {
            auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
            if (kwType == beginSy || kwType == ifSy || kwType == whileSy ||
                kwType == repeatSy || kwType == forSy) {
                ComplexOperator(followers);
            }
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::SimpleOperator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Accept(ttIdent);
        if (EqualKeyWord(assignSy)) {
            AssignOperator(followers);
        }
        else {
            ProcedureOperator(followers);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::AssignOperator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Accept(assignSy);
        Expression(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Expression(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto simpleExpressionFollowers = followers;
    simpleExpressionFollowers.insert(simpleExpressionFollowers.end(), { 
        make_shared<CKeyWordToken>(equalSy),
        make_shared<CKeyWordToken>(laterSy),
        make_shared<CKeyWordToken>(greaterSy),
        make_shared<CKeyWordToken>(laterequalSy),
        make_shared<CKeyWordToken>(greaterequalSy),
        make_shared<CKeyWordToken>(latergreaterSy),
        });
    try {
        SimpleExpression(simpleExpressionFollowers);
        if (IsRelOper()) {
            GetNextToken();
            SimpleExpression(followers);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::SimpleExpression(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto summandFollowers = followers;
    summandFollowers.insert(summandFollowers.end(), {
        make_shared<CKeyWordToken>(plusSy),
        make_shared<CKeyWordToken>(minusSy),
        make_shared<CKeyWordToken>(orSy),
        });
    try {
        if (IsSign()) {
            GetNextToken();
        }
        Summand(summandFollowers);
        while (IsAddOper()) {
            GetNextToken();
            Summand(summandFollowers);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Summand(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto multipliertFollowers = followers;
    multipliertFollowers.insert(multipliertFollowers.end(), {
        make_shared<CKeyWordToken>(starSy),
        make_shared<CKeyWordToken>(slashSy),
        make_shared<CKeyWordToken>(divSy),
        make_shared<CKeyWordToken>(modSy),
        make_shared<CKeyWordToken>(andSy),
        });
    try {
        Multiplier(multipliertFollowers);
        while (IsMultOper()) {
            GetNextToken();
            Multiplier(multipliertFollowers);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Multiplier(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto expressionFollowers = followers;
    expressionFollowers.push_back(make_shared<CKeyWordToken>(rightparSy));
    try {
        if (curToken->GetType() == ttIdent || curToken->GetType() == ttConst) {
            GetNextToken();
        }
        else if (EqualKeyWord(leftparSy)) {
            GetNextToken();
            Expression(expressionFollowers);
            Accept(rightparSy);
        }
        else if (EqualKeyWord(notSy)) {
            GetNextToken();
            Multiplier(followers);
        }
        else {
            lexer->GetIOPtr()->AddError(e021);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ProcedureOperator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto actualParameterFollowers = followers;
    actualParameterFollowers.push_back(make_shared<CKeyWordToken>(rightparSy));
    actualParameterFollowers.push_back(make_shared<CKeyWordToken>(commaSy));
    try {
        if (EqualKeyWord(leftparSy)) {
            GetNextToken();
            ActualParameter(actualParameterFollowers);
            while (EqualKeyWord(commaSy)) {
                GetNextToken();
                ActualParameter(actualParameterFollowers);
            }
            Accept(rightparSy);
        }
        else {
            return;
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ActualParameter(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        Expression(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ComplexOperator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
        switch (kwType) {
        case beginSy:
            CompOperator(followers);
            break;
        case ifSy:
            ConditionalOperator(followers);
            break;
        case whileSy:
            LoopWithPrecondition(followers);
            break;
        case repeatSy:
            LoopWithPostcondition(followers);
            break;
        case forSy:
            LoopWithParameter(followers);
            break;
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ConditionalOperator(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto expressionFollowers = followers;
    expressionFollowers.push_back(make_shared<CKeyWordToken>(thenSy));
    auto operatorFollowers = followers;
    operatorFollowers.push_back(make_shared<CKeyWordToken>(elseSy));
    try {
        GetNextToken();
        Expression(expressionFollowers);
        Accept(thenSy);
        Operator(operatorFollowers);
        if (EqualKeyWord(elseSy)) {
            GetNextToken();
            Operator(followers);
        }
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::LoopWithPrecondition(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto expressionFollowers = followers;
    expressionFollowers.push_back(make_shared<CKeyWordToken>(doSy));
    try {
        GetNextToken();
        Expression(expressionFollowers);
        Accept(doSy);
        Operator(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::LoopWithPostcondition(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto operatorFollowers = followers;
    operatorFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    operatorFollowers.push_back(make_shared<CKeyWordToken>(untilSy));
    try {
        GetNextToken();
        Operator(operatorFollowers);
        while (EqualKeyWord(semicolonSy)) {
            GetNextToken();
            Operator(operatorFollowers);
        }
        Accept(untilSy);
        Expression(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::LoopWithParameter(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    auto expressionFollowers1 = followers;
    expressionFollowers1.push_back(make_shared<CKeyWordToken>(toSy));
    expressionFollowers1.push_back(make_shared<CKeyWordToken>(downtoSy));
    auto expressionFollowers2 = followers;
    expressionFollowers2.push_back(make_shared<CKeyWordToken>(doSy));
    try {
        GetNextToken();
        Accept(ttIdent);
        Accept(assignSy);
        Expression(expressionFollowers1);
        Dirrection(followers);
        Expression(expressionFollowers2);
        Accept(doSy);
        Operator(followers);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::Dirrection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        if (EqualKeyWord(toSy) || EqualKeyWord(downtoSy)) {
            GetNextToken();
            return;
        }
        lexer->GetIOPtr()->AddError(e055);
    }
    catch (...) { catchedError = true; }
    if (!Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

CParser::CParser(shared_ptr<CLexer> _lexer) : lexer(move(_lexer)) { }

shared_ptr<CLexer> CParser::GetLexerPtr()
{
    return lexer;
}

void CParser::Evaluate()
{
    GetNextToken();
    Program({ make_shared<CEmptyToken>() });
}