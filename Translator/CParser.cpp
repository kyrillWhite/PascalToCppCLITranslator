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

void CParser::AddIdent(string name, shared_ptr<CType> type)
{
    if (name != "") {
        auto _type = scopes->GetCurrent()->AddIdent(name, type);
        if (!_type) {
            try { lexer->GetIOPtr()->AddError(e101); }
            catch (...) { }
        }
    }
}

shared_ptr<CType> CParser::DeclaringCheckIdent()
{
    if (curToken->GetType() == ttIdent) {
        auto type = scopes->GetCurrent()->LookupIdent(dynamic_cast<CIdentToken*>(curToken.get())->ToString());
        if (!type) {
            try { lexer->GetIOPtr()->AddError(e104); }
            catch (...) {}
        }
        return type;
    }
    return nullptr;
}

std::string CParser::GetIdentName()
{
    return curToken->GetType() == ttIdent ? dynamic_cast<CIdentToken*>(curToken.get())->ToString() : "";
}

std::shared_ptr<CType> CParser::GetTypeByName(std::string name)
{
    auto it = typeNames.find(name);
    if (it != typeNames.end()) {
        switch (it->second) {
        case ttInteger:
            return make_shared<CIntType>();
        case ttReal:
            return make_shared<CRealType>();
        case ttBoolean:
            return make_shared<CBooleanType>();
        case ttString:
            return make_shared<CStringType>();
        }
    }
    auto type = scopes->GetCurrent()->LookupIdent(name);
    if (type && type->GetType() == ttCustom) {
        return make_shared<CCustomType>(type, false);
    }
    try {
        lexer->GetIOPtr()->AddError(e104);
    }
    catch (...) {}
    return make_shared<CNameType>();
}

std::shared_ptr<CType> CParser::GetConstantType()
{
    if (curToken->GetType() != ttConst) {
        return nullptr;
    }
    auto constType = dynamic_cast<CConstToken*>(curToken.get())->GetVariant()->GetVariantType();
    switch (constType) {
    case vtInt:
        return make_shared<CIntType>();
    case vtReal:
        return make_shared<CRealType>();
    case vtString:
        return make_shared<CStringType>();
    case vtBoolean:
        return make_shared<CBooleanType>();
    }
    return nullptr;
}

void CParser::AcceptProcedureCallTypes(shared_ptr<CType> proc, vector<shared_ptr<CType>> attributes)
{
    if (!proc || proc->GetType() != ttProcedure) {
        return;
    }
    auto procedure = dynamic_pointer_cast<CProcedureType>(proc);
    auto procedureAttributes = procedure->GetAttributes();
    int attrCount = min(procedureAttributes.size(), attributes.size());
    if (procedureAttributes.size() != attributes.size()) {
        try { lexer->GetIOPtr()->AddError(e198); }
        catch (...) {}
    }
    for (int i = 0; i < attrCount; i++) {
        auto attribute = attributes[i];
        auto procAttribute = procedureAttributes[i];
        if (!IsEqualTypes(attribute, procAttribute)) {
            try { lexer->GetIOPtr()->AddError(e199); }
            catch (...) {}
        }
        if (procAttribute->GetRef() && !attribute->IsIdent()) {
            try { lexer->GetIOPtr()->AddError(e215); }
            catch (...) {}
        }
    }
}

bool CParser::IsVariableType(shared_ptr<CType> type)
{
    if (!type) {
        return false;
    }
    auto eType = type->GetType();
    return eType == ttInteger || eType == ttReal || 
        eType == ttBoolean || eType == ttString || 
        (eType == ttCustom && 
            !dynamic_pointer_cast<CCustomType>(type)->IsType() &&
            IsVariableType(dynamic_pointer_cast<CCustomType>(type)->GetOriginType()));
}

bool CParser::IsEqualTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2)
{
    auto _type1 = type1;
    auto _type2 = type2;
    if (type1 && type1->GetType() == ttCustom) {
        _type1 = dynamic_pointer_cast<CCustomType>(type1)->GetOriginType();
    }
    if (type2 && type2->GetType() == ttCustom) {
        _type2 = dynamic_pointer_cast<CCustomType>(type2)->GetOriginType();
    }
    return _type1 && _type2 && _type1->GetType() == _type2->GetType();
}

bool CParser::IsScalarTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2)
{
    auto _type1 = type1;
    auto _type2 = type2;
    if (type1 && type1->GetType() == ttCustom) {
        _type1 = dynamic_pointer_cast<CCustomType>(type1)->GetOriginType();
    }
    if (type2 && type2->GetType() == ttCustom) {
        _type2 = dynamic_pointer_cast<CCustomType>(type2)->GetOriginType();
    }
    return _type1 && _type2 && (_type1->GetType() == ttInteger || _type1->GetType() == ttReal) && 
        (_type2->GetType() == ttInteger || _type2->GetType() == ttReal);
}

bool CParser::IsScalarType(std::shared_ptr<CType> type)
{
    auto _type = type;
    if (type && type->GetType() == ttCustom) {
        _type = dynamic_pointer_cast<CCustomType>(type)->GetOriginType();
    }
    return _type && (_type->GetType() == ttInteger || _type->GetType() == ttReal);
}

bool CParser::IsBooleanTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2)
{
    auto _type1 = type1;
    auto _type2 = type2;
    if (type1 && type1->GetType() == ttCustom) {
        _type1 = dynamic_pointer_cast<CCustomType>(type1)->GetOriginType();
    }
    if (type2 && type2->GetType() == ttCustom) {
        _type2 = dynamic_pointer_cast<CCustomType>(type2)->GetOriginType();
    }
    return _type1 && _type2 && _type1->GetType() == ttBoolean && _type2->GetType() == ttBoolean;
}

bool CParser::IsBooleanType(std::shared_ptr<CType> type)
{
    auto _type = type;
    if (type && type->GetType() == ttCustom) {
        _type = dynamic_pointer_cast<CCustomType>(type)->GetOriginType();
    }
    return _type && _type->GetType() == ttBoolean;
}

bool CParser::IsIntegerTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2)
{
    auto _type1 = type1;
    auto _type2 = type2;
    if (type1 && type1->GetType() == ttCustom) {
        _type1 = dynamic_pointer_cast<CCustomType>(type1)->GetOriginType();
    }
    if (type2 && type2->GetType() == ttCustom) {
        _type2 = dynamic_pointer_cast<CCustomType>(type2)->GetOriginType();
    }
    return _type1 && _type2 && _type1->GetType() == ttInteger && _type2->GetType() == ttInteger;
}

bool CParser::IsIntegerAndReal(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2)
{
    auto _type1 = type1;
    auto _type2 = type2;
    if (type1 && type1->GetType() == ttCustom) {
        _type1 = dynamic_pointer_cast<CCustomType>(type1)->GetOriginType();
    }
    if (type2 && type2->GetType() == ttCustom) {
        _type2 = dynamic_pointer_cast<CCustomType>(type2)->GetOriginType();
    }
    return _type1 && _type2 && (_type1->GetType() == ttInteger && _type2->GetType() == ttReal || 
        _type1->GetType() == ttReal && _type2->GetType() == ttInteger);
}

bool CParser::IsIntegerType(std::shared_ptr<CType> type)
{
    auto _type = type;
    if (type && type->GetType() == ttCustom) {
        _type = dynamic_pointer_cast<CCustomType>(type)->GetOriginType();
    }
    return _type && _type->GetType() == ttInteger;
}

bool CParser::IsStringTypes(std::shared_ptr<CType> type1, std::shared_ptr<CType> type2)
{
    auto _type1 = type1;
    auto _type2 = type2;
    if (type1 && type1->GetType() == ttCustom) {
        _type1 = dynamic_pointer_cast<CCustomType>(type1)->GetOriginType();
    }
    if (type2 && type2->GetType() == ttCustom) {
        _type2 = dynamic_pointer_cast<CCustomType>(type2)->GetOriginType();
    }
    return _type1 && _type2 && _type1->GetType() == ttString && _type2->GetType() == ttString;
}

bool CParser::IsRelOper()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    bool is = kwType == equalSy || kwType == laterSy || kwType == greaterSy ||
        kwType == laterequalSy || kwType == greaterequalSy || kwType == latergreaterSy;
    if (is) {
        switch (kwType)
        {
        case equalSy:
            generator->Equal();
            break;
        case laterSy:
            generator->Later();
            break;
        case greaterSy:
            generator->Greater();
            break;
        case laterequalSy:
            generator->Laterequal();
            break;
        case greaterequalSy:
            generator->Greaterequal();
            break;
        case latergreaterSy:
            generator->NotEqual();
            break;
        }
    }
    return is;
}

bool CParser::IsSign()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    bool is = kwType == plusSy || kwType == minusSy;
    if (is) {
        switch (kwType)
        {
        case plusSy:
            generator->Plus();
            break;
        case minusSy:
            generator->Minus();
            break;
        }
    }
    return is;
}

bool CParser::IsAddOper()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    bool is = kwType == plusSy || kwType == minusSy || kwType == orSy;
    if (is) {
        switch (kwType)
        {
        case plusSy:
            generator->Add();
            break;
        case minusSy:
            generator->Sub();
            break;
        case orSy:
            generator->Or();
            break;
        }
    }
    return is;
}

bool CParser::IsMultOper()
{
    if (curToken->GetType() != ttKeyword) {
        return false;
    }
    auto kwType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
    bool is = kwType == starSy || kwType == slashSy ||
        kwType == divSy || kwType == modSy || kwType == andSy;
    if (is) {
        switch (kwType)
        {
        case starSy:
            generator->Mult();
            break;
        case slashSy:
            generator->Div();
            break;
        case divSy:
            generator->Div();
            break;
        case modSy:
            generator->Mod();
            break;
        case andSy:
            generator->And();
            break;
        }
    }
    return is;
}

bool CParser::EqualKeyWord(EKeyWordType kwType)
{
    return curToken->GetType() == ttKeyword &&
        dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType() == kwType;
}

void CParser::Program(vector<shared_ptr<CToken>> followers)
{
    string progName;
    scopes->Push(make_shared<CScope>());
    auto writeType = make_shared<CProcedureType>();
    writeType->SetAttributes({ make_shared<CNameType>() });
    AddIdent("write", writeType);
    bool catchedError = false;
    auto blockFollowers = followers;
    blockFollowers.push_back(make_shared<CKeyWordToken>(pointSy));
    try {
        Accept(programSy);
        progName = GetIdentName();
        Accept(ttIdent); 
        AddIdent(progName, make_shared<CNameType>());
        Accept(semicolonSy);
        Block(progName, blockFollowers);
        Accept(pointSy);
    }
    catch (...) { }
    scopes->Pop();
    generator->MainFunc(progName);
}

void CParser::Block(string callingName, vector<shared_ptr<CToken>> followers)
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
        generator->OpenBlock(callingName);
        TypesSection(typesSectionFollowers);
        VariablesSection(variablesSectionFollowers);
        ProceduresSection(proceduresSectionFollowers);
        generator->OperatorSectionOpen(callingName);
        OperatorsSection(followers);
        generator->CloseBlock();
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        string identName = GetIdentName();
        Accept(ttIdent);
        Accept(equalSy);
        auto sourceType = GetTypeByName(curToken ->ToString());
        auto type = make_shared<CCustomType>(sourceType, true);
        Type(followers);
        AddIdent(identName, type);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        vector<string> names;
        names.push_back(GetIdentName());
        Accept(ttIdent);
        while (EqualKeyWord(commaSy)) {
            GetNextToken();
            names.push_back(GetIdentName());
            Accept(ttIdent);
        }
        Accept(colonSy);
        auto type = GetTypeByName(curToken->ToString());
        for (string name : names) {
            AddIdent(name, type);
        }
        generator->SameTypeVars(names, type);
        Type(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

void CParser::ProcedureDefinition(vector<shared_ptr<CToken>> followers)
{
    auto procedureHeaderFollowers = followers;
    procedureHeaderFollowers.push_back(make_shared<CKeyWordToken>(varSy));
    procedureHeaderFollowers.push_back(make_shared<CKeyWordToken>(procedureSy));
    procedureHeaderFollowers.push_back(make_shared<CKeyWordToken>(beginSy));
    bool catchedError = false;
    try {
        GetNextToken();
        string identName = GetIdentName();
        Accept(ttIdent);
        auto procedureType = make_shared<CProcedureType>();
        AddIdent(identName, procedureType);
        scopes->Push(make_shared<CScope>());
        auto parametersTypes = ProcedureHeader(procedureHeaderFollowers);
        procedureType->SetAttributes(parametersTypes);
        Block(identName, followers);
        scopes->Pop();
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

vector<shared_ptr<CType>> CParser::ProcedureHeader(vector<shared_ptr<CToken>> followers)
{
    vector<shared_ptr<CType>> parametersTypes;
    bool catchedError = false;
    auto formalParametersSectionFollowers = followers;
    formalParametersSectionFollowers.push_back(make_shared<CKeyWordToken>(semicolonSy));
    formalParametersSectionFollowers.push_back(make_shared<CKeyWordToken>(rightparSy));
    try {
        if (EqualKeyWord(leftparSy)) {
            Accept(leftparSy);
            auto sectionTypes = FormalParametersSection(formalParametersSectionFollowers);
            parametersTypes.insert(parametersTypes.end(), sectionTypes.begin(), sectionTypes.end());
            while (EqualKeyWord(semicolonSy)) {
                Accept(semicolonSy);
                generator->ParameterDivider();
                sectionTypes = FormalParametersSection(formalParametersSectionFollowers);
                parametersTypes.insert(parametersTypes.end(), sectionTypes.begin(), sectionTypes.end());
            }
            Accept(rightparSy);
        }
        Accept(semicolonSy);
        generator->ProcedureHeaderPush();
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return parametersTypes;
}

vector<shared_ptr<CType>> CParser::FormalParametersSection(vector<shared_ptr<CToken>> followers)
{
    vector<shared_ptr<CType>> parametersTypes;
    bool catchedError = false;
    try {
        if (EqualKeyWord(varSy)) {
            GetNextToken();
            parametersTypes = ParametersGroup(true, followers);
            for (auto parType : parametersTypes) {
                parType->SetRef(true);
            }
        }
        else {
            parametersTypes = ParametersGroup(false, followers);
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return parametersTypes;
}

vector<shared_ptr<CType>> CParser::ParametersGroup(bool isRef, vector<shared_ptr<CToken>> followers)
{
    vector<shared_ptr<CType>> parametersTypes;
    bool catchedError = false;
    try {
        vector<string> names;
        names.push_back(GetIdentName());
        Accept(ttIdent);
        while (EqualKeyWord(commaSy)) {
            GetNextToken();
            names.push_back(GetIdentName());
            Accept(ttIdent);
        }
        Accept(colonSy);
        auto type = GetTypeByName(curToken->ToString());
        for (int i = 0; i < names.size(); i++) {
            AddIdent(names[i], type);
            parametersTypes.push_back(type);
            generator->ProcedureParameter(names[i], type, isRef);
            if (i != names.size() - 1) {
                generator->ParameterDivider();
            }
        }
        Type(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return parametersTypes;
}

void CParser::OperatorsSection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        CompOperator(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        generator->CompOperatorOpen();
        Operator(operatorFollowers);
        while (EqualKeyWord(semicolonSy)) {
            GetNextToken();
            Operator(operatorFollowers);
        }
        Accept(endSy);
        generator->CompOperatorClose();
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
            generator->OperatorClose();
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
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        auto type = DeclaringCheckIdent();
        string name = curToken->ToString();
        Accept(ttIdent);
        if (EqualKeyWord(assignSy)) {
            if (!IsVariableType(type)) {
                try { lexer->GetIOPtr()->AddError(e100); }
                catch (...) {}
            }
            generator->Assign(name);
            auto nType = AssignOperator(followers);
            if (!IsEqualTypes(type, nType)) {
                try { lexer->GetIOPtr()->AddError(e182); }
                catch (...) {}
            }
        }
        else {
            if (!type || type->GetType() != ttProcedure) {
                try { lexer->GetIOPtr()->AddError(e100); }
                catch (...) {}
            }
            generator->CallProcedure(name);
            generator->LeftPar();

            auto attributes = ProcedureOperator(followers);
            if (name == "write") {
                if (attributes.size() != 1) {
                    try { lexer->GetIOPtr()->AddError(e198); }
                    catch (...) { }
                }
            }
            else {
                AcceptProcedureCallTypes(type, attributes);
            }
            generator->RightPar();
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

shared_ptr<CType> CParser::AssignOperator(vector<shared_ptr<CToken>> followers)
{
    shared_ptr<CType> type;
    bool catchedError = false;
    try {
        Accept(assignSy);
        type = Expression(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return type;
}

shared_ptr<CType> CParser::Expression(vector<shared_ptr<CToken>> followers)
{
    shared_ptr<CType> type;
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
        type = SimpleExpression(simpleExpressionFollowers);
        if (IsRelOper()) {
            type->SetIsIdent(false);
            GetNextToken();
            auto nType = SimpleExpression(followers);
            if (!IsScalarTypes(type, nType) && !IsStringTypes(type, nType)) {
                try { lexer->GetIOPtr()->AddError(e186); }
                catch (...) {}
            }
            type = make_shared<CBooleanType>();
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return type;
}

shared_ptr<CType> CParser::SimpleExpression(vector<shared_ptr<CToken>> followers)
{
    shared_ptr<CType> type;
    bool catchedError = false;
    auto summandFollowers = followers;
    summandFollowers.insert(summandFollowers.end(), {
        make_shared<CKeyWordToken>(plusSy),
        make_shared<CKeyWordToken>(minusSy),
        make_shared<CKeyWordToken>(orSy),
        });
    try {
        bool hasSign = false;
        if (IsSign()) {
            GetNextToken();
            hasSign = true;
        }
        type = Summand(summandFollowers);
        if (hasSign && !IsScalarType(type)) {
            try { lexer->GetIOPtr()->AddError(e184); }
            catch (...) {}
        }
        if (hasSign) {
            type->SetIsIdent(false);
        }
        while (IsAddOper()) {
            type->SetIsIdent(false);
            auto operType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
            GetNextToken();
            auto nType = Summand(summandFollowers);
            if (operType == orSy) {
                if (!IsBooleanTypes(type, nType)) {
                    try { lexer->GetIOPtr()->AddError(e210); }
                    catch (...) {}
                }
            }
            else if (operType == plusSy || operType == minusSy) {
                if (!IsScalarTypes(type, nType) && !IsStringTypes(type, nType)) {
                    try { lexer->GetIOPtr()->AddError(e211); }
                    catch (...) {}
                }
            }
            if (IsIntegerAndReal(type, nType)) {
                type = make_shared<CRealType>();
            }
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return type;
}

shared_ptr<CType> CParser::Summand(vector<shared_ptr<CToken>> followers)
{
    shared_ptr<CType> type;
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
        type = Multiplier(multipliertFollowers);
        while (IsMultOper()) {
            type->SetIsIdent(false);
            auto operType = dynamic_cast<CKeyWordToken*>(curToken.get())->GetKeyWordType();
            GetNextToken();
            auto nType = Multiplier(multipliertFollowers);
            if (operType == andSy) {
                if (!IsBooleanTypes(type, nType)) {
                    try { lexer->GetIOPtr()->AddError(e210); }
                    catch (...) {}
                }
            }
            else if (operType == divSy || operType == modSy) {
                if (!IsIntegerTypes(type, nType)) {
                    try { lexer->GetIOPtr()->AddError(e212); }
                    catch (...) {}
                }
            }
            else if (operType == starSy) {
                if (!IsScalarTypes(type, nType)) {
                    try { lexer->GetIOPtr()->AddError(e213); }
                    catch (...) {}
                }
            }
            else if (operType == slashSy) {
                if (!IsScalarTypes(type, nType)) {
                    try { lexer->GetIOPtr()->AddError(e214); }
                    catch (...) {}
                }
            }
            if ((operType == starSy || operType == slashSy) && IsIntegerAndReal(type, nType)) {
                type = make_shared<CRealType>();
            }
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return type;
}

shared_ptr<CType> CParser::Multiplier(vector<shared_ptr<CToken>> followers)
{
    shared_ptr<CType> type;
    bool catchedError = false;
    auto expressionFollowers = followers;
    expressionFollowers.push_back(make_shared<CKeyWordToken>(rightparSy));
    try {
        if (curToken->GetType() == ttIdent) {
            type = scopes->GetCurrent()->LookupIdent(curToken->ToString()); 
            type->SetIsIdent(true);
            if (!IsVariableType(type)) {
                try { lexer->GetIOPtr()->AddError(e100); }
                catch (...) {}
            }
            DeclaringCheckIdent();
            generator->Ident(curToken->ToString());
            GetNextToken();
        }
        else if (curToken->GetType() == ttConst) {
            type = GetConstantType();
            generator->Const(curToken->ToString(), type);
            GetNextToken();
        }
        else if (EqualKeyWord(leftparSy)) {
            GetNextToken();
            generator->LeftPar();
            type = Expression(expressionFollowers);
            Accept(rightparSy);
            generator->RightPar();
        }
        else if (EqualKeyWord(notSy)) {
            generator->Not();
            GetNextToken();
            type = Multiplier(followers);
            if (!IsBooleanType(type)) {
                try { lexer->GetIOPtr()->AddError(e210); }
                catch (...) {}
            }
        }
        else {
            lexer->GetIOPtr()->AddError(e021);
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return type;
}

vector<shared_ptr<CType>> CParser::ProcedureOperator(vector<shared_ptr<CToken>> followers)
{
    vector<shared_ptr<CType>> attributes;
    bool catchedError = false;
    auto actualParameterFollowers = followers;
    actualParameterFollowers.push_back(make_shared<CKeyWordToken>(rightparSy));
    actualParameterFollowers.push_back(make_shared<CKeyWordToken>(commaSy));
    try {
        if (EqualKeyWord(leftparSy)) {
            GetNextToken();
            auto attriblute = ActualParameter(actualParameterFollowers);
            attributes.push_back(attriblute);
            while (EqualKeyWord(commaSy)) {
                GetNextToken();
                generator->ParameterDividerCallProc();
                auto attriblute = ActualParameter(actualParameterFollowers);
                attributes.push_back(attriblute);
            }
            Accept(rightparSy);
        }
        else {
            return attributes;
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return attributes;
}

shared_ptr<CType> CParser::ActualParameter(vector<shared_ptr<CToken>> followers)
{
    shared_ptr<CType> type;
    bool catchedError = false;
    try {
        type = Expression(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return type;
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
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        generator->If();
        generator->LeftPar();
        auto type = Expression(expressionFollowers);
        generator->RightPar();
        if (!IsBooleanType(type)) {
            try { lexer->GetIOPtr()->AddError(e135); }
            catch (...) {}
        }
        Accept(thenSy);
        Operator(operatorFollowers);
        if (EqualKeyWord(elseSy)) {
            generator->Else();
            GetNextToken();
            Operator(followers);
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        generator->LoopWithPrecondition();
        generator->LeftPar();
        auto type = Expression(expressionFollowers);
        generator->RightPar();
        if (!IsBooleanType(type)) {
            try { lexer->GetIOPtr()->AddError(e135); }
            catch (...) {}
        }
        Accept(doSy);
        Operator(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        generator->LoopWithPostcondition();
        Operator(operatorFollowers);
        while (EqualKeyWord(semicolonSy)) {
            GetNextToken();
            Operator(operatorFollowers);
        }
        Accept(untilSy);
        generator->LoopWithPrecondition();
        generator->LeftPar();
        auto type = Expression(followers);
        generator->RightPar();
        generator->OperatorClose();
        if (!IsBooleanType(type)) {
            try { lexer->GetIOPtr()->AddError(e135); }
            catch (...) {}
        }
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
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
        auto type = DeclaringCheckIdent();
        if (!IsIntegerType(type) && !IsBooleanType(type)) {
            try { lexer->GetIOPtr()->AddError(e143); }
            catch (...) {}
        }
        string parameter = curToken->ToString();
        Accept(ttIdent);
        Accept(assignSy);
        generator->ForInit(parameter);
        auto fType = Expression(expressionFollowers1);
        if (!IsEqualTypes(type, fType)) {
            try { lexer->GetIOPtr()->AddError(e182); }
            catch (...) {}
        }
        bool dir = Dirrection(followers);
        generator->ForCondition(parameter, dir);
        auto tType = Expression(expressionFollowers2);
        if (!IsEqualTypes(type, tType)) {
            try { lexer->GetIOPtr()->AddError(e182); }
            catch (...) {}
        }
        generator->ForIteration(parameter, dir);
        Accept(doSy);
        Operator(followers);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
}

bool CParser::Dirrection(vector<shared_ptr<CToken>> followers)
{
    bool catchedError = false;
    try {
        if (EqualKeyWord(toSy)) {
            GetNextToken();
            return true;
        }
        else if (EqualKeyWord(downtoSy)) {
            GetNextToken();
            return false;
        }
        lexer->GetIOPtr()->AddError(e055);
    }
    catch (...) { catchedError = true; }
    if (!isNeutralizeErrors && catchedError) {
        throw exception();
    }
    if (isNeutralizeErrors && !Belong(followers)) {
        if (!catchedError) {
            try { lexer->GetIOPtr()->AddError(e006); }
            catch (...) {}
        }
        SkipTo(followers);
    }
    return false;
}

CParser::CParser(shared_ptr<CLexer> _lexer, std::shared_ptr<CGenerator> _generator, bool _isNeutralizeErrors) :
    lexer(_lexer), generator(_generator), isNeutralizeErrors(_isNeutralizeErrors)
{
    scopes = make_unique<CScopes>();
    typeNames =
    {
        { "integer", ttInteger },
        { "real",    ttReal },
        { "boolean", ttBoolean },
        { "string",  ttString },
    };
}

shared_ptr<CLexer> CParser::GetLexerPtr()
{
    return lexer;
}

void CParser::Evaluate()
{
    GetNextToken();
    Program({ make_shared<CEmptyToken>() });
}