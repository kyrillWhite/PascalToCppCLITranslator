#include "CIO.h"

using namespace std;

void CIO::ReadString()
{
    getline(*inputStream, buffer);
    buffer += '\n';
}

CIO::CIO(std::ifstream& input)
{
    inputStream.reset(&input);
    outputStream = make_unique<ostringstream>();
}

CIO::CIO(std::string input)
{
    inputStream = make_unique<istringstream>(input);
    outputStream = make_unique<ostringstream>();
}

CIO::CIO(ifstream &input, ostream &output)
{
    inputStream.reset(&input);
    outputStream.reset(&output);
}

CIO::CIO(string input, ostream &output)
{
    inputStream = make_unique<istringstream>(input);
    outputStream.reset(&output);
}

CIO::~CIO()
{
    if (dynamic_cast<ifstream*>(inputStream.get())) {
        inputStream.release();
    }
    if (!dynamic_cast<ostringstream*>(outputStream.get())) {
        outputStream.release();
    }
}

char CIO::GetNextChar()
{
    while (symbolNumber == buffer.size()) {
        if (inputStream->eof()) {
            buffer = "";
            lineNumber++;
            symbolNumber = 0;
            return ' ';
        }
        ReadString();
        lineNumber++;
        symbolNumber = 0;
    }
    return buffer[symbolNumber++];
}

void CIO::AddError(EErrorType eType)
{
    errors.push_back(make_unique<CError>(lineNumber, symbolNumber, eType));
    throw errors[errors.size() - 1].get();
}

void CIO::PrintErrors()
{
    for (auto &error : errors) {
        *outputStream << error->GetText() << " (стр " << error->GetLineNum() << ", стлб "
            << error->GetSymbolNum() << ")"  << endl;
    }
}

bool CIO::IsEndOfInput()
{
    return inputStream->eof() && buffer == "";
}

std::string CIO::GetOutputString()
{
    auto os = dynamic_cast<ostringstream*>(outputStream.get());
    if (os) {
        return os->str();
    }
    return "";
}
