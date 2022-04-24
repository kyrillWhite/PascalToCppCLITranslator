#include "CGenerator.h"

using namespace std;

std::string CGenerator::ConvertType(std::shared_ptr<CType> type)
{
    if (!type) {
        return "";
    }
    auto _type = type;
    if (type->GetType() == ttCustom) {
        _type = dynamic_pointer_cast<CCustomType>(type)->GetOriginType();
    }
    switch (_type->GetType()) {
    case ttBoolean:
        return "bool";
    case ttReal:
        return "float";
    case ttString:
        return "std::string";
    case ttInteger:
        return "int";
    default:
        return "";
    }
}

void CGenerator::OpenBlock(std::string name)
{
    *outputStream << "namespace " << name << "\n{\n";
}

void CGenerator::CloseBlock()
{
    *outputStream << "}\n\n";
}

void CGenerator::SameTypeVars(std::vector<std::string> names, std::shared_ptr<CType> type)
{
    *outputStream << ConvertType(type) << " ";
    for (int i = 0; i < names.size(); i++) {
        *outputStream << names[i] << (i != names.size() - 1 ? ", " : "");
    }
    *outputStream << ";\n";
}

void CGenerator::OperatorSectionOpen(std::string name)
{
    string parameters = ProcedureHeaderPop();
    *outputStream << "void " << name << "(" << parameters << ")\n";
}

void CGenerator::CompOperatorOpen()
{
    *outputStream << "{\n";
}

void CGenerator::CompOperatorClose()
{
    *outputStream << "}\n";
}

void CGenerator::OperatorClose()
{
    *outputStream << ";\n";
}

void CGenerator::Assign(std::string name)
{
    *outputStream << name << " = ";
}

void CGenerator::Const(std::string con, std::shared_ptr<CType> type)
{
    *outputStream << (type->GetType() == ttString ? "\"" + con + "\"" : con);
}

void CGenerator::Ident(std::string name)
{
    *outputStream << name;
}

void CGenerator::Add()
{
    *outputStream << "+";
}

void CGenerator::Sub()
{
    *outputStream << "-";
}

void CGenerator::Or()
{
    *outputStream << "||";
}

void CGenerator::Mult()
{
    *outputStream << "*";
}

void CGenerator::Div()
{
    *outputStream << "/";
}

void CGenerator::Mod()
{
    *outputStream << "%";
}

void CGenerator::And()
{
    *outputStream << "&&";
}

void CGenerator::Not()
{
    *outputStream << "!";
}

void CGenerator::Plus()
{
    *outputStream << "+";
}

void CGenerator::Minus()
{
    *outputStream << "-";
}

void CGenerator::Equal()
{
    *outputStream << "==";
}

void CGenerator::Later()
{
    *outputStream << "<";
}

void CGenerator::Greater()
{
    *outputStream << ">";
}

void CGenerator::Laterequal()
{
    *outputStream << "<=";
}

void CGenerator::Greaterequal()
{
    *outputStream << ">=";
}

void CGenerator::NotEqual()
{
    *outputStream << "!=";
}

void CGenerator::LeftPar()
{
    *outputStream << "(";
}

void CGenerator::RightPar()
{
    *outputStream << ")";
}

void CGenerator::ProcedureParameter(std::string name, std::shared_ptr<CType> type, bool isRef)
{
    procedureHeader += ConvertType(type) + (isRef ? "& " : " ") + name;
}

void CGenerator::ParameterDivider()
{
    procedureHeader += ", ";
}

void CGenerator::ParameterDividerCallProc()
{
    *outputStream << ", ";
}

void CGenerator::ProcedureHeaderPush()
{
    procedureParameters.push(procedureHeader);
}

string CGenerator::ProcedureHeaderPop()
{
    if (procedureParameters.empty()) {
        return "";
    }
    string top = procedureParameters.top();
    procedureParameters.pop();
    return top;
}

void CGenerator::CallProcedure(std::string name)
{
    if (name == "write") {
        *outputStream << "std::cout << ";
    }
    else {
        *outputStream << name << "::" << name;
    }
}

void CGenerator::If()
{
    *outputStream << "if ";
}

void CGenerator::Else()
{
    *outputStream << "else ";
}

void CGenerator::LoopWithPrecondition()
{
    *outputStream << "while ";
}

void CGenerator::LoopWithPostcondition()
{
    *outputStream << "do\n";
}

void CGenerator::ForInit(std::string parameter)
{
    *outputStream << "for (" << parameter << " = ";
}

void CGenerator::ForCondition(std::string parameter, bool dir)
{
    *outputStream << "; " << parameter << (dir ? " <= " : " >= ");
}

void CGenerator::ForIteration(std::string parameter, bool dir)
{
    *outputStream << "; " << parameter << (dir ? "++" : "--") << ")\n";
}


void CGenerator::MainFunc(std::string name)
{
    *outputStream << 
        "int main()\n"
        "{\n" << name << "::" << name << "();\n"
        "System::Console::ReadKey();\n"
        "}";
}

CGenerator::CGenerator(std::ostream& output)
{
    outputStream.reset(&output);
    *outputStream << "#include <string>\n#include <iostream>\n\n";
}

CGenerator::~CGenerator()
{
    if (!dynamic_cast<ostringstream*>(outputStream.get())) {
        outputStream.release();
    }
}