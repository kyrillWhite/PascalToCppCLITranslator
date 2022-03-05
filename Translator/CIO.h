#pragma once
#include <istream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "CError.h"

class CIO
{
private:
    std::unique_ptr<std::istream> inputStream;
    std::unique_ptr<std::ostream> outputStream;

    int lineNumber = 0;
    int symbolNumber = 0;
    std::string buffer; 

    std::vector<std::unique_ptr<CError>> errors;

    void ReadString();

public:
    CIO(std::ifstream &input);
    CIO(std::string input);
    CIO(std::ifstream &input, std::ostream &output);
    CIO(std::string input, std::ostream &output);
    ~CIO();

    char GetNextChar();
    void AddError(EErrorType eType);
    void PrintErrors();
    bool IsEndOfInput();
    std::string GetOutputString();
};