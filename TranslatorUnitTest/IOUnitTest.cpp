#include <iostream>

#include "CppUnitTest.h"
#include "../Translator/CIO.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TranslatorUnitTest
{
	TEST_CLASS(IOUnitTest)
	{
	public:

		#define CURRENT_DIRECTORY GetDirectoryName(__FILE__)
		string GetDirectoryName(string path) {
			const size_t lsIdx = path.rfind('\\');
			return string::npos != lsIdx ? path.substr(0, lsIdx + 1) : "";
		}

		TEST_METHOD(IFileOString) {
			ifstream input(CURRENT_DIRECTORY + "tests/IO/test1.txt");
			auto IO = make_unique<CIO>(input);
			char ch = IO->GetNextChar();
			Assert::AreEqual(ch, 't');
			try {
				IO->AddError(e006);
			}
			catch (...) {
				IO->PrintErrors();
			}
			string output = IO->GetOutputString();
			Assert::AreEqual(output.c_str(), "Запрещенный символ (стр 1, стлб 1)\n");
			input.close();
		}

		TEST_METHOD(IStringOString) {
			string input("t");
			auto IO = make_unique<CIO>(input);
			char ch = IO->GetNextChar();
			Assert::AreEqual(ch, 't');
			try {
				IO->AddError(e006);
			}
			catch (...) { }
			IO->PrintErrors();
			string output = IO->GetOutputString();
			Assert::AreEqual(output.c_str(), "Запрещенный символ (стр 1, стлб 1)\n");
		}

		TEST_METHOD(IFileOFile) {
			ifstream input(CURRENT_DIRECTORY + "tests/IO/test1.txt");
			ofstream output(CURRENT_DIRECTORY + "tests/IO/output1.txt");
			auto IO = make_unique<CIO>(input, output);
			char ch = IO->GetNextChar();
			Assert::AreEqual(ch, 't');
			try {
				IO->AddError(e006);
			}
			catch (...) { }
			IO->PrintErrors();
			output.close();
			ifstream outputResIn(CURRENT_DIRECTORY + "tests/IO/output1.txt");
			string outputRes;
			getline(outputResIn, outputRes);
			Assert::AreEqual(outputRes.c_str(), "Запрещенный символ (стр 1, стлб 1)");
			input.close();
			outputResIn.close();
		}

		TEST_METHOD(IStringOFile) {
			string input("t");
			ofstream output(CURRENT_DIRECTORY + "tests/IO/output1.txt");
			auto IO = make_unique<CIO>(input, output);
			char ch = IO->GetNextChar();
			Assert::AreEqual(ch, 't');
			try {
				IO->AddError(e006);
			}
			catch (...) { }
			IO->PrintErrors();
			output.close();
			ifstream outputResIn(CURRENT_DIRECTORY + "tests/IO/output1.txt");
			string outputRes;
			getline(outputResIn, outputRes);
			Assert::AreEqual(outputRes.c_str(), "Запрещенный символ (стр 1, стлб 1)");
			outputResIn.close();
		}

		TEST_METHOD(ReadString) {
			string input("s1\ns2");
			auto IO = make_unique<CIO>(input);
			string res = "";
			for (int i = 0; i < 7; i++) {
				res += IO->GetNextChar();
			}
			Assert::AreEqual(res.c_str(), "s1\ns2\n ");
		}

		TEST_METHOD(PrintErrors) {
			string input("");
			auto IO = make_unique<CIO>(input);
			try {
				IO->AddError(e006);
			}
			catch (...) { }
			try {
				IO->AddError(e075);
			}
			catch (...) {}
			IO->PrintErrors();
			string output = IO->GetOutputString();
			Assert::AreEqual(output.c_str(), "Запрещенный символ (стр 0, стлб 0)\nОшибка в символьной константе (стр 0, стлб 0)\n");
		}

		TEST_METHOD(IsEndOfInput) {
			string input("");
			auto IO = make_unique<CIO>(input);
			IO->GetNextChar();
			IO->GetNextChar();
			Assert::IsTrue(IO->IsEndOfInput());
		}

		TEST_METHOD(ClassError) {
			auto error = make_unique<CError>(1, 2, e006);
			Assert::AreEqual(error->GetLineNum(), 1);
			Assert::AreEqual(error->GetSymbolNum(), 2);
			Assert::AreEqual(error->GetText().c_str(), "Запрещенный символ");
		}
	};
}
