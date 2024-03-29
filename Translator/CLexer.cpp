#include <algorithm>
#include "CLexer.h"

using namespace std;

CLexer::CLexer(shared_ptr<CIO> _IO) : IO(move(_IO)), ch(' ')
{
    keyWords =
    {
        {"program",   programSy},
        {"type",      typeSy},
        {"var",       varSy},
        {"procedure", procedureSy},
        {"or",        orSy},
        {"and",       andSy},
        {"not",       notSy},
        {"div",       divSy},
        {"mod",       modSy},
        {"begin",     beginSy},
        {"end",       endSy},
        {"if",        ifSy},
        {"then",      thenSy},
        {"else",      elseSy},
        {"while",     whileSy},
        {"do",        doSy},
        {"repeat",    repeatSy},
        {"until",     untilSy},
        {"for",       forSy},
        {"to",        toSy},
        {"downto",    downtoSy}
    };
}

shared_ptr<CIO> CLexer::GetIOPtr()
{
    return IO;
}

unique_ptr<CToken> CLexer::GetNextToken()
{
    switch (ch) {
    case '*':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(starSy);
    case '/':
        ch = IO->GetNextChar();
        if (ch == '/') {
            while (ch != '\n') {
                if (IO->IsEndOfInput()) {
                    return make_unique<CEmptyToken>();
                }
                ch = IO->GetNextChar();
            }
            ch = IO->GetNextChar();
            return GetNextToken();
        }
        else {
            return make_unique<CKeyWordToken>(slashSy);
        }
    case '=':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(equalSy);
    case ',':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(commaSy);
    case ';':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(semicolonSy);
    case ':':
        ch = IO->GetNextChar();
        if (ch == '=') {
            ch = IO->GetNextChar();
            return make_unique<CKeyWordToken>(assignSy);
        }
        else {
            return make_unique<CKeyWordToken>(colonSy);
        }
    case '.':
        ch = IO->GetNextChar();
        if (ch == '.') {
            ch = IO->GetNextChar();
            return make_unique<CKeyWordToken>(twopointsSy);
        }
        else {
            return make_unique<CKeyWordToken>(pointSy);
        }
    case '^':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(arrowSy);
    case '(':
        ch = IO->GetNextChar();
        if (ch == '*') {
            char tCh = ' ';
            while (tCh != '*' || ch != ')') {
                if (IO->IsEndOfInput()) {
                    IO->AddError(e086);
                }
                tCh = ch;
                ch = IO->GetNextChar();
            }
            ch = IO->GetNextChar();
            return GetNextToken();
        }
        else {
            return make_unique<CKeyWordToken>(leftparSy);
        }
    case ')':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(rightparSy);
    case '{':
        while (ch != '}') {
            if (IO->IsEndOfInput()) {
                IO->AddError(e086);
            }
            ch = IO->GetNextChar();
        }
        ch = IO->GetNextChar();
        return GetNextToken();
    case '[':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(lbracketSy);
    case ']':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(rbracketSy);
    case '<':
        ch = IO->GetNextChar();
        if (ch == '=') {
            ch = IO->GetNextChar();
            return make_unique<CKeyWordToken>(laterequalSy);
        }
        else if (ch == '>') {
            ch = IO->GetNextChar();
            return make_unique<CKeyWordToken>(latergreaterSy);
        }
        else {
            return make_unique<CKeyWordToken>(laterSy);
        }
    case '>':
        ch = IO->GetNextChar();
        if (ch == '=') {
            ch = IO->GetNextChar();
            return make_unique<CKeyWordToken>(greaterequalSy);
        }
        else {
            return make_unique<CKeyWordToken>(greaterSy);
        }
    case '+':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(plusSy);
    case '-':
        ch = IO->GetNextChar();
        return make_unique<CKeyWordToken>(minusSy);
    case '\'': {
        ch = IO->GetNextChar();
        string constStr = "";
        while (ch != '\'') {
            if (ch == '\n' || IO->IsEndOfInput()) {
                IO->AddError(e077);
            }
            constStr += ch;
            ch = IO->GetNextChar();
        }
        if (constStr == "") {
            ch = IO->GetNextChar();
            IO->AddError(e075);
        }
        ch = IO->GetNextChar();
        return make_unique<CConstToken>(make_unique<CStringVariant>(constStr));
    }
    default:
        if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_') {
            string word = "";
            while (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' ||
                ch == '_' || ch >= '0' && ch <= '9') {
                word += ch;
                ch = IO->GetNextChar();
            }
            string lcWord = word;
            transform(lcWord.begin(), lcWord.end(), lcWord.begin(),
                [](unsigned char c) { return std::tolower(c); });
            if (lcWord == "true") {
                return make_unique<CConstToken>(make_unique<CBooleanVariant>(true));
            }
            else if (lcWord == "false") {
                return make_unique<CConstToken>(make_unique<CBooleanVariant>(false));
            }

            auto it = keyWords.find(lcWord);
            if (it == keyWords.end()) {
                return make_unique<CIdentToken>(lcWord);
            }
            else {
                return make_unique<CKeyWordToken>(it->second);
            }
        }
        else if (ch >= '0' && ch <= '9') {
            bool hasDot = false;
            bool onlyZerosBeforeDot = true;
            string num = "";
            while (ch >= '0' && ch <= '9' || ch == '.') {
                if (ch == '.') {
                    if (hasDot) {
                        ch = IO->GetNextChar();
                        IO->AddError(e201);
                    }
                    hasDot = true;
                }
                else if (ch != '0' && !hasDot) {
                    onlyZerosBeforeDot = false;
                }
                num += ch;
                ch = IO->GetNextChar();
            }
            if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_') {
                IO->AddError(e050);
            }
            else if (hasDot) {
                try {
                    double dNum = stod(num);
                    return make_unique<CConstToken>(make_unique<CRealVariant>(dNum));
                }
                catch (...) {
                    if (onlyZerosBeforeDot) {
                        IO->AddError(e206);
                    }
                    else {
                        IO->AddError(e207);
                    }
                }
            }
            else {
                try {
                    int iNum = stoi(num);
                    if (iNum > 2147483647) {
                        IO->AddError(e203);
                    }
                    return make_unique<CConstToken>(make_unique<CIntVariant>(iNum));
                }
                catch (...) {
                    IO->AddError(e203);
                }
            }
        }
        else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\f' ||
            ch == '\r' || ch == '\v' || ch == '\0') {
            while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\f' ||
                ch == '\r' || ch == '\v' || ch == '\0') {
                if (IO->IsEndOfInput()) {
                    return make_unique<CEmptyToken>();
                }
                ch = IO->GetNextChar();
            }
            return GetNextToken();
        }
        else {
            ch = IO->GetNextChar();
            IO->AddError(e006);
        }
    }
}
