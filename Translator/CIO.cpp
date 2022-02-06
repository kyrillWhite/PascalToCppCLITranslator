#include "CIO.h"

using namespace std;

void CIO::ReadString()
{
    getline(*inputStream, buffer);
}

bool CIO::IsEndOfInput()
{
    return (*inputStream).eof() && symbolNumber == buffer.size();
}

CIO::CIO(std::ifstream &input)
{
    inputStream.reset(&input);
    outputStream = make_unique<stringstream>();
}

CIO::CIO(string input)
{
    inputStream = make_unique<istringstream>(input);
    outputStream = make_unique<stringstream>();
}

CIO::CIO(ifstream &input, ofstream &output)
{
    inputStream.reset(&input);
    outputStream.reset(&output);
}

CIO::CIO(string input, ofstream &output)
{
    inputStream = make_unique<istringstream>(input);
    outputStream.reset(&output);
}

CIO::~CIO()
{
    if (dynamic_cast<ifstream*>(inputStream.get())) {
        inputStream.release();
    }
    if (dynamic_cast<ofstream*>(outputStream.get())) {
        outputStream.release();
    }
}

bool CIO::GetNextChar(char& ch)
{
    while (symbolNumber == buffer.size()) {
        ReadString();
        lineNumber++;
        symbolNumber = 0;
    }
    ch = buffer[symbolNumber++];
    return !IsEndOfInput();
}

void CIO::AddError(ErrorType eType)
{
    errors.push_back(make_unique<CError>(lineNumber, symbolNumber, eType));
}

void CIO::PrintErrors()
{
    for (auto &error : errors) {
        *outputStream << "(" << error->GetLineNum() << ", "
            << error->GetSymbolNum() << ") " << error->GetText() << endl;
    }
}
