#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <memory>

#include "CBooleanType.h"
#include "CCustomType.h"
#include "CIntType.h"
#include "CNameType.h"
#include "CRealType.h"
#include "CStringType.h"
#include "CProcedureType.h"

class CGenerator
{
private:
    std::unique_ptr<std::ostream> outputStream;
    std::stack<std::string> procedureParameters;
    std::string procedureHeader;

    std::string ConvertType(std::shared_ptr<CType> type);

public:
    void OpenBlock(std::string name);
    void CloseBlock();
    void SameTypeVars(std::vector<std::string> names, std::shared_ptr<CType> type);
    void OperatorSectionOpen(std::string name);
    void CompOperatorOpen();
    void CompOperatorClose();
    void OperatorClose();
    void Assign(std::string name);
    void CallProcedure(std::string name);

    void If();
    void Else();
    void LoopWithPrecondition();
    void LoopWithPostcondition();
    void ForInit(std::string parameter);
    void ForCondition(std::string parameter, bool dir);
    void ForIteration(std::string parameter, bool dir);

    void Const(std::string con, std::shared_ptr<CType> type);
    void Ident(std::string name);

    void Add();
    void Sub();
    void Or();
    void Mult();
    void Div();
    void Mod();
    void And();
    void Not();

    void Plus();
    void Minus();
    
    void Equal();
    void Later();
    void Greater();
    void Laterequal();
    void Greaterequal();
    void NotEqual();

    void LeftPar();
    void RightPar();

    void ProcedureParameter(std::string name, std::shared_ptr<CType> type, bool isRef);
    void ParameterDivider();
    void ParameterDividerCallProc();
    void ProcedureHeaderPush();
    std::string ProcedureHeaderPop();

    void MainFunc(std::string name);

    CGenerator(std::ostream& output);

    ~CGenerator();
};