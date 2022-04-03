#include <iostream>

#include "CppUnitTest.h"
#include "../Translator/CLexer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TranslatorUnitTest
{
	TEST_CLASS(LexerUnitTest)
	{
	public:

		TEST_METHOD(OneSymbolKeyWordToken)
		{
			string input = "*/=,;:.^()[]><+-";

			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == starSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == slashSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == equalSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == commaSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == semicolonSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == colonSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == pointSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == arrowSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == leftparSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == rightparSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == lbracketSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == rbracketSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == greaterSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == laterSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == plusSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == minusSy);
		}

		TEST_METHOD(TwoSymbolsKeyWordToken) {
			string input = "<=>=<>:=..";

			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == laterequalSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == greaterequalSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == latergreaterSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == assignSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == twopointsSy);
		}

		TEST_METHOD(WordKeyWordToken) {
			string input = "program var procedure or and not div mod begin " 
				"end if then else while do repeat until for to downto";

			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == programSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == varSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == procedureSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == orSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == andSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == notSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == divSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == modSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == beginSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == endSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == ifSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == thenSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == elseSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == whileSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == doSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == repeatSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == untilSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == forSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == toSy);
			Assert::IsTrue(dynamic_cast<CKeyWordToken*>(lexer->GetNextToken().get())->GetKeyWordType() == downtoSy);
		}

		TEST_METHOD(Commentaries) {
			string input = 
				"//commentary1\n"
				"{commentary2}\n"
				"(*commentary3*)";

			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			Assert::AreEqual(lexer->GetNextToken()->ToString().c_str(), "");
		}

		TEST_METHOD(Constants) {
			string input =
				"\'string\'\n"
				"true\n"
				"false\n"
				"2000\n"
				"123.456";

			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			Assert::AreEqual(dynamic_cast<CStringVariant*>(dynamic_cast<CConstToken*>(lexer->GetNextToken().get())->GetVariant())->GetValue().c_str(), "string");
			Assert::AreEqual(dynamic_cast<CBooleanVariant*>(dynamic_cast<CConstToken*>(lexer->GetNextToken().get())->GetVariant())->GetValue(), true);
			Assert::AreEqual(dynamic_cast<CBooleanVariant*>(dynamic_cast<CConstToken*>(lexer->GetNextToken().get())->GetVariant())->GetValue(), false);
			Assert::AreEqual(dynamic_cast<CIntVariant*>(dynamic_cast<CConstToken*>(lexer->GetNextToken().get())->GetVariant())->GetValue(), 2000);
			Assert::AreEqual(dynamic_cast<CRealVariant*>(dynamic_cast<CConstToken*>(lexer->GetNextToken().get())->GetVariant())->GetValue(), 123.456);
		}

		TEST_METHOD(Identificators) {
			string input =
				"ident1\n"
				"ident2\n";

			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			Assert::AreEqual(lexer->GetNextToken().get()->ToString().c_str(), "ident1");
			Assert::AreEqual(lexer->GetNextToken().get()->ToString().c_str(), "ident2");
		}

		string ErrorTestOutput(string input) {
			auto IO = make_shared<CIO>(input);
			auto lexer = make_shared<CLexer>(IO);

			try {
				lexer->GetNextToken();
			}
			catch (...) {}
			lexer->GetIOPtr()->PrintErrors();
			return lexer->GetIOPtr()->GetOutputString();
		}

		TEST_METHOD(Error_e006) {
			string input = "$";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Запрещенный символ (стр 1, стлб 1)\n");
		}

		TEST_METHOD(Error_e050) {
			string input = "1a";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Ошибка в константе (стр 1, стлб 2)\n");
		}

		TEST_METHOD(Error_e075) {
			string input = "\'\'";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Ошибка в символьной константе (стр 1, стлб 2)\n");
		}

		TEST_METHOD(Error_e077) {
			string input = "\'string";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Строковая константа не закрыта (стр 1, стлб 8)\n");
		}

		TEST_METHOD(Error_e086) {
			string input1 = "{comment";
			string input2 = "(*comment";

			Assert::AreEqual(ErrorTestOutput(input1).c_str(), "Комментарий не закрыт (стр 2, стлб 0)\n");
			Assert::AreEqual(ErrorTestOutput(input2).c_str(), "Комментарий не закрыт (стр 2, стлб 0)\n");
		}

		TEST_METHOD(Error_e201) {
			string input = "123.3.9";
			
			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Ошибка в вещественной константе: должна идти цифра (стр 1, стлб 6)\n");
		}

		TEST_METHOD(Error_e203) {
			string input = "1000000000000000000";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Целая константа превышает предел (стр 1, стлб 20)\n");
		}

		TEST_METHOD(Error_e206) {
			string input = "0.00000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000009";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Слишком маленькая вещественная константа (стр 1, стлб 362)\n");
		}

		TEST_METHOD(Error_e207) {
			string input = "1000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"000000000000000000000000000000000000000000"
				"0000000000000000000000000000000000.9";

			Assert::AreEqual(ErrorTestOutput(input).c_str(), "Слишком большая вещественная константа (стр 1, стлб 362)\n");
		}
	};
}
