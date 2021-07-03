#pragma once

#include "../Combinator/Rule.h"

#include <string>
#include <stack>
#include <sstream>
#include <vector>

namespace byx
{
	// Parserµ¥Ôª²âÊÔ
	class ParserTest
	{
	public:
		static void Run();
	private:
		static void ParseResultTest();
		static void EmptyTest();
		static void AnyTest();
		static void CharTest();
		static void CharSetTest();
		static void ExcludeTest();
		static void RangeTest();
		static void PrefixTest();
		static void PrefixSetTest();
		static void ConcatTest();
		static void ChooseOneTest();
		static void ZeroOrMoreTest();
		static void OneOrMoreTest();
		static void CallbackOnSuccessTest();
		static void CallbackOnFailTest();
		static void LazyTest();
		static void SimpleExpressionEvaluatorTest();
		static void SimpleLexerTest();

		class SimpleLexer
		{
		public:
			static std::vector<std::pair<std::string, bool>> Lex(const std::string& input);
		private:
			static std::vector<std::pair<std::string, bool>> tokens;
			static Rule blank, dot, digit, integer, decimal, rule;
			static void addInteger(ParseResult r);
			static void addDecimal(ParseResult r);
		};

		class SimpleExpressionEvaluator
		{
		public:
			static int Eval(const std::string& input);
		private:
			static std::stack<int> s;
			static Rule digit;
			static Rule integer;
			static Rule add, sub, mul, div, lp, rp;
			static Rule factor, term, expr, lazyExpr;
			static Rule LazyExpr();
			static void PushInt(ParseResult r);
			static void Calc(ParseResult r);
		};
	};
}