#include <iostream>

#include "CppUnitTest.h"
#include "../Translator/CParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TranslatorUnitTest
{
	TEST_CLASS(ParserUnitTest)
	{
	public:
		string ErrorTestOutput(string input) {
			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);
			auto parser = make_shared<CParser>(lexer);

			try {
				parser->Evaluate();
			}
			catch (...) {}
			parser->GetLexerPtr()->GetIOPtr()->PrintErrors();
			return lexer->GetIOPtr()->GetOutputString();
		}

		TEST_METHOD(Program) {
			string correct = ErrorTestOutput("program pr1; begin end.");
			string error1 = ErrorTestOutput("_ pr1; begin end.");
			string error2 = ErrorTestOutput("program program begin end.");
			string error3 = ErrorTestOutput("program pr1 _ begin end.");
			string error4 = ErrorTestOutput("program pr1; begin end _");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно быть служебное слово PROGRAM (стр 1, стлб 2)\n");
			Assert::AreEqual(error2.c_str(), "Должно идти имя (стр 1, стлб 16)\n");
			Assert::AreEqual(error3.c_str(), "Должен идти символ \';\' (стр 1, стлб 14)\n");
			Assert::AreEqual(error4.c_str(), "Должен идти символ \'.\' (стр 1, стлб 25)\n");
		}

		TEST_METHOD(TypesSection) {
			string correct = ErrorTestOutput("program pr1; type t1 = real; t2 = integer; begin end.");
			string error1 = ErrorTestOutput("program pr1; type t1 = real _ begin end.");
			string error2 = ErrorTestOutput("program pr1; type t1 = real; t2 = integer _ begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должен идти символ \';\' (стр 1, стлб 30)\n");
			Assert::AreEqual(error2.c_str(), "Должен идти символ \';\' (стр 1, стлб 44)\n");
		}

		TEST_METHOD(TypeDefinition) {
			string correct = ErrorTestOutput("program pr1; type t1 = real; begin end.");
			string error1 = ErrorTestOutput("program pr1; type type = real; begin end.");
			string error2 = ErrorTestOutput("program pr1; type t1 _ real; begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти имя (стр 1, стлб 23)\n");
			Assert::AreEqual(error2.c_str(), "Должен идти символ \'=\' (стр 1, стлб 23)\n");
		}

		TEST_METHOD(Type) {
			string correct = ErrorTestOutput("program pr1; type t1 = real; begin end.");
			string error1 = ErrorTestOutput("program pr1; type t1 = type; begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти имя (стр 1, стлб 28)\n");
		}

		TEST_METHOD(VariablesSection) {
			string correct = ErrorTestOutput("program pr1; var v1, v2: real; v3, v4: real; begin end.");
			string error1 = ErrorTestOutput("program pr1; var v1, v2: real _ v3, v4: real; begin end.");
			string error2 = ErrorTestOutput("program pr1; var v1, v2: real; v3, v4: real _ begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должен идти символ \';\' (стр 1, стлб 32)\n");
			Assert::AreEqual(error2.c_str(), "Должен идти символ \';\' (стр 1, стлб 46)\n");
		}

		TEST_METHOD(SameTypeVars) {
			string correct = ErrorTestOutput("program pr1; var v1, v2: real; begin end.");
			string error1 = ErrorTestOutput("program pr1; var var, v2: real; begin end.");
			string error2 = ErrorTestOutput("program pr1; var v1, var: real; begin end.");
			string error3 = ErrorTestOutput("program pr1; var v1, v2 _ real; begin end.");
			string error4 = ErrorTestOutput("program pr1; var v1, v2: var; begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти имя (стр 1, стлб 21)\n");
			Assert::AreEqual(error2.c_str(), "Должно идти имя (стр 1, стлб 25)\n");
			Assert::AreEqual(error3.c_str(), "Должен идти символ \':\' (стр 1, стлб 26)\n");
			Assert::AreEqual(error4.c_str(), "Должно идти имя (стр 1, стлб 29)\n");
		}

		TEST_METHOD(ProceduresSection) {
			string correct = ErrorTestOutput("program pr1; procedure p1; begin end; begin end.");
			string error1 = ErrorTestOutput("program pr1; procedure p1; begin end _ begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должен идти символ \';\' (стр 1, стлб 39)\n");
		}

		TEST_METHOD(ProcedureHeader) {
			string correct = ErrorTestOutput(
				"program pr1;\n" 
				"procedure p1(par1: real; par2: real);\n"
				"begin end;\n" 
				"begin end.");
			string error1 = ErrorTestOutput(
				"program pr1;\n"
				"procedure var(par1: real; par2: real);\n"
				"begin end;\n"
				"begin end.");
			string error2 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1: real; par2: real _;\n"
				"begin end;\n"
				"begin end.");
			string error3 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1: real; par2: real) _\n"
				"begin end;\n"
				"begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти имя (стр 2, стлб 14)\n");
			Assert::AreEqual(error2.c_str(), "Должен идти символ \')\' (стр 2, стлб 38)\n");
			Assert::AreEqual(error3.c_str(), "Должен идти символ \';\' (стр 2, стлб 39)\n");
		}

		TEST_METHOD(FormalParametersSection) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1: real; var par1: real; procedure p2, p3);\n"
				"begin end;\n"
				"begin end.");
			string error1 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1: real; var par1: real; procedure,);\n"
				"begin end;\n"
				"begin end.");
			string error2 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1: real; var par1: real; procedure p2,,);\n"
				"begin end;\n"
				"begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти имя (стр 2, стлб 52)\n");
			Assert::AreEqual(error2.c_str(), "Должно идти имя (стр 2, стлб 56)\n");
		}

		TEST_METHOD(ParametersGroup) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1, par2: real);\n"
				"begin end;\n"
				"begin end.");
			string error1 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(type: real);\n"
				"begin end;\n"
				"begin end.");
			string error2 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1, type: real);\n"
				"begin end;\n"
				"begin end.");
			string error3 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1, par2 _ real);\n"
				"begin end;\n"
				"begin end.");
			string error4 = ErrorTestOutput(
				"program pr1;\n"
				"procedure p1(par1, par2: type);\n"
				"begin end;\n"
				"begin end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти имя (стр 2, стлб 18)\n");
			Assert::AreEqual(error2.c_str(), "Должно идти имя (стр 2, стлб 24)\n");
			Assert::AreEqual(error3.c_str(), "Должен идти символ \':\' (стр 2, стлб 26)\n");
			Assert::AreEqual(error4.c_str(), "Должно идти имя (стр 2, стлб 30)\n");
		}

		TEST_METHOD(CompOperator) {
			string correct = ErrorTestOutput("program pr1; begin a := 1; a := 2; end.");
			string error1 = ErrorTestOutput("program pr1; _ a := 1; a := 2; end.");
			string error2 = ErrorTestOutput("program pr1; begin a := 1; a := 2; _.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должно идти слово BEGIN (стр 1, стлб 15)\n");
			Assert::AreEqual(error2.c_str(), "Должно идти слово END (стр 1, стлб 38)\n");
		}

		TEST_METHOD(Expression) {
			string correct = ErrorTestOutput("program pr1; begin a := 2; a := a > 2; end.");

			Assert::AreEqual(correct.c_str(), "");
		}

		TEST_METHOD(SimpleExpression) {
			string correct = ErrorTestOutput("program pr1; begin a := -1 + 1 + 9; end.");

			Assert::AreEqual(correct.c_str(), "");
		}

		TEST_METHOD(Summand) {
			string correct = ErrorTestOutput("program pr1; begin a := 2 * 3 + 1 * 6; end.");

			Assert::AreEqual(correct.c_str(), "");
		}

		TEST_METHOD(Multiplier) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"a := a;\n"
				"a := 1;\n"
				"a := (1 + 1);\n"
				"a := not (1 < 2);\n"
				"end.");
			string error = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"a := a - * b;\n"
				"end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error.c_str(), "Требуется выражение (стр 3, стлб 11)\n");
		}

		TEST_METHOD(ProcedureOperator) {
			string correct = ErrorTestOutput("program pr1; begin p1; p2(1 + 1, a); end.");
			string error1 = ErrorTestOutput("program pr1; begin p1(); end.");
			string error2 = ErrorTestOutput("program pr1; begin p1(a; end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Требуется выражение (стр 1, стлб 24)\n");
			Assert::AreEqual(error2.c_str(), "Должен идти символ \')\' (стр 1, стлб 25)\n");
		}

		TEST_METHOD(ConditionalOperator) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"if (1 > 2) then\n"
				"a := a\n"
				"else\n"
				"a := 1 + 1\n"
				"end.");
			string error = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"if (1 > 2) else\n"
				"a := a\n"
				"else\n"
				"a := 1 + 1\n"
				"end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error.c_str(), "Должно идти слово THEN (стр 3, стлб 16)\n");
		}

		TEST_METHOD(LoopWithPrecondition) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"while (1 > 2) do\n"
				"a := a\n"
				"end.");
			string error = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"while (1 > 2) while\n"
				"a := a\n"
				"end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error.c_str(), "Должно идти слово DO (стр 3, стлб 20)\n");
		}

		TEST_METHOD(LoopWithPostcondition) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"repeat\n"
				"a := a\n"
				"until (1 > 2)\n"
				"end.");
			string error = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"repeat\n"
				"a := a\n"
				"var (1 > 2)\n"
				"end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error.c_str(), "Должно идти слово UNTIL (стр 5, стлб 4)\n");
		}

		TEST_METHOD(LoopWithParameter) {
			string correct = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"for a := 1 to 20 do\n"
				"b := b\n"
				"end.");
			string error1 = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"for a _ 1 to 20 do\n"
				"b := b\n"
				"end.");
			string error2 = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"for a := 1 _ 20 do\n"
				"b := b\n"
				"end.");
			string error3 = ErrorTestOutput(
				"program pr1;\n"
				"begin\n"
				"for a := 1 to 20 _\n"
				"b := b\n"
				"end.");

			Assert::AreEqual(correct.c_str(), "");
			Assert::AreEqual(error1.c_str(), "Должен идти символ \':=\' (стр 3, стлб 8)\n");
			Assert::AreEqual(error2.c_str(), "Должно идти слово TO или DOWNTO (стр 3, стлб 13)\n");
			Assert::AreEqual(error3.c_str(), "Должно идти слово DO (стр 3, стлб 19)\n");
		}
	};
}