#pragma once
#include <istream>
#include <fstream>
#include <sstream>
#include <string>

class CIO
{
private:
    std::unique_ptr<std::istream> inputStream;
    std::unique_ptr<std::ostream> outputStream;

    int lineNumber = 0;
    int symbolNumber = 0;

    std::string buffer;

    bool IsEndOfInput();
    void ReadString();

public:
    CIO(std::ifstream &input);
    CIO(std::string input);
    CIO(std::ifstream &input, std::ofstream &output);
    CIO(std::string input, std::ofstream &output);
    ~CIO();
    bool GetNextChar(char &ch);
};