#include "CombinatorTest.h"
#include "../Combinator/ParseResult.h"
#include "../Combinator/ParserCombinator.h"
#include "../Combinator/Rule.h"

#include <cassert>
#include <sstream>
#include <stack>

using namespace std;
using namespace byx;

// ParserTest

void byx::ParserTest::Run()
{
	ParseResultTest();
	EmptyTest();
	AnyTest();
	CharTest();
	CharSetTest();
	ExcludeTest();
	RangeTest();
	PrefixTest();
	PrefixSetTest();
	ConcatTest();
	ChooseOneTest();
	ZeroOrMoreTest();
	OneOrMoreTest();
	CallbackOnSuccessTest();
	CallbackOnFailTest();
	LazyTest();

	SimpleExpressionEvaluatorTest();
	SimpleLexerTest();
}

void byx::ParserTest::ParseResultTest()
{
	ParseResult res = ParseResult::CreateSucceed("12", "345");
	assert(res);
	assert(res.isSucceeded());
	assert(res.getRecognized() == "12");
	assert(res.getRemaining() == "345");

	res = ParseResult::CreateFail();
	assert(!res);
	assert(!res.isSucceeded());
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "");
}

void byx::ParserTest::EmptyTest()
{
	ParseResult res = Empty().parse("123");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "123");

	res = Empty().parse("");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "");
}

void byx::ParserTest::AnyTest()
{
	ParseResult res = Any().parse("123");
	assert(res);
	assert(res.getRecognized() == "1");
	assert(res.getRemaining() == "23");

	res = Any().parse("");
	assert(!res);
}

void byx::ParserTest::CharTest()
{
	ParseResult res = Char('a').parse("abc");
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "bc");

	res = Char('a').parse("123");
	assert(!res);

	res = Char('a').parse("a");
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "");

	res = Char('a').parse("1");
	assert(!res);

	res = Char('a').parse("");
	assert(!res);
}

void byx::ParserTest::CharSetTest()
{
	ParseResult res = CharSet("abc").parse("apple");
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "pple");

	res = CharSet("abc").parse("banana");
	assert(res);
	assert(res.getRecognized() == "b");
	assert(res.getRemaining() == "anana");

	res = CharSet("abc").parse("c");
	assert(res);
	assert(res.getRecognized() == "c");
	assert(res.getRemaining() == "");

	res = CharSet("abc").parse("dog");
	assert(!res);

	res = CharSet("abc").parse("");
	assert(!res);

	res = CharSet("").parse("123");
	assert(!res);

	res = CharSet("").parse("");
	assert(!res);
}

void byx::ParserTest::ExcludeTest()
{
	ParseResult res = Exclude("abc").parse("apple");
	assert(!res);

	res = Exclude("abc").parse("banana");
	assert(!res);

	res = Exclude("abc").parse("c");
	assert(!res);

	res = Exclude("abc").parse("dance");
	assert(res);
	assert(res.getRecognized() == "d");
	assert(res.getRemaining() == "ance");

	res = Exclude("").parse("dance");
	assert(res);
	assert(res.getRecognized() == "d");
	assert(res.getRemaining() == "ance");

	res = Exclude("abc").parse("");
	assert(!res);

	res = Exclude("").parse("");
	assert(!res);
}

void byx::ParserTest::RangeTest()
{
	ParseResult res = Range('a', 'c').parse("abc");
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "bc");

	res = Range('a', 'c').parse("bac");
	assert(res);
	assert(res.getRecognized() == "b");
	assert(res.getRemaining() == "ac");

	res = Range('a', 'c').parse("ccb");
	assert(res);
	assert(res.getRecognized() == "c");
	assert(res.getRemaining() == "cb");

	res = Range('a', 'c').parse("dcb");
	assert(!res);

	res = Range('c', 'a').parse("abc");
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "bc");

	res = Range('c', 'a').parse("bac");
	assert(res);
	assert(res.getRecognized() == "b");
	assert(res.getRemaining() == "ac");

	res = Range('c', 'a').parse("ccb");
	assert(res);
	assert(res.getRecognized() == "c");
	assert(res.getRemaining() == "cb");

	res = Range('c', 'a').parse("dcb");
	assert(!res);

	res = Range('1', '1').parse("123");
	assert(res);
	assert(res.getRecognized() == "1");
	assert(res.getRemaining() == "23");

	res = Range('1', '1').parse("234");
	assert(!res);

	res = Range('a', 'z').parse("");
	assert(!res);
}

void byx::ParserTest::PrefixTest()
{
	ParseResult res = Prefix("byx").parse("byx2000");
	assert(res);
	assert(res.getRecognized() == "byx");
	assert(res.getRemaining() == "2000");

	res = Prefix("byx").parse("byx");
	assert(res);
	assert(res.getRecognized() == "byx");
	assert(res.getRemaining() == "");

	res = Prefix("byx").parse("lzy2000");
	assert(!res);

	res = Prefix("byx").parse("by2000");
	assert(!res);

	res = Prefix("byx").parse("by");
	assert(!res);

	res = Prefix("byx").parse("");
	assert(!res);

	res = Prefix("").parse("abc");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "abc");

	res = Prefix("").parse("");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "");
}

void byx::ParserTest::PrefixSetTest()
{
	ParseResult res = PrefixSet({ "int", "double", "char" }).parse("int i = 0;");
	assert(res);
	assert(res.getRecognized() == "int");
	assert(res.getRemaining() == " i = 0;");

	res = PrefixSet({ "int", "double", "char" }).parse("double");
	assert(res);
	assert(res.getRecognized() == "double");
	assert(res.getRemaining() == "");

	res = PrefixSet({ "int", "double", "char" }).parse("charc='2';");
	assert(res);
	assert(res.getRecognized() == "char");
	assert(res.getRemaining() == "c='2';");

	res = PrefixSet({ "int", "double", "char" }).parse("string s = \"123\";");
	assert(!res);

	res = PrefixSet({ "int", "double", "char" }).parse("doubl x = 37.5");
	assert(!res);

	res = PrefixSet({ "int", "double", "char" }).parse("");
	assert(!res);

	res = PrefixSet({ }).parse("doubl x = 37.5");
	assert(!res);

	res = PrefixSet({ }).parse("");
	assert(!res);
}

void byx::ParserTest::ConcatTest()
{
	ParseResult res = Concat{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("abcde");
	assert(res);
	assert(res.getRecognized() == "abc");
	assert(res.getRemaining() == "de");

	res = Concat{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("abdef");
	assert(!res);

	res = Concat{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("xyzef");
	assert(!res);

	res = Concat{ make_shared<Char>('1') }.parse("123");
	assert(res);
	assert(res.getRecognized() == "1");
	assert(res.getRemaining() == "23");

	res = Concat{ make_shared<Char>('4') }.parse("123");
	assert(!res);

	res = Concat{ }.parse("123");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "123");

	res = Concat{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("");
	assert(!res);

	res = Concat{ }.parse("");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "");
}

void byx::ParserTest::ChooseOneTest()
{
	ParseResult res = ChooseOne{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("apple");
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "pple");

	res = ChooseOne{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("banana");
	assert(res);
	assert(res.getRecognized() == "b");
	assert(res.getRemaining() == "anana");

	res = ChooseOne{ make_shared<Char>('a'), make_shared<Char>('b'), make_shared<Char>('c') }.parse("c");
	assert(res);
	assert(res.getRecognized() == "c");
	assert(res.getRemaining() == "");

	res = ChooseOne{  }.parse("123");
	assert(!res);
	res = ChooseOne{  }.parse("");
	assert(!res);

}

void byx::ParserTest::ZeroOrMoreTest()
{
	ParseResult res = ZeroOrMore(make_shared<Char>('a')).parse("aaaaa");
	assert(res);
	assert(res.getRecognized() == "aaaaa");
	assert(res.getRemaining() == "");

	res = ZeroOrMore(make_shared<Char>('a')).parse("aaaaab");
	assert(res);
	assert(res.getRecognized() == "aaaaa");
	assert(res.getRemaining() == "b");

	res = ZeroOrMore(make_shared<Char>('a')).parse("baaaaa");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "baaaaa");

	res = ZeroOrMore(make_shared<Char>('a')).parse("");
	assert(res);
	assert(res.getRecognized() == "");
	assert(res.getRemaining() == "");
}

void byx::ParserTest::OneOrMoreTest()
{
	ParseResult res = OneOrMore(make_shared<Char>('a')).parse("aaaaa");
	assert(res);
	assert(res.getRecognized() == "aaaaa");
	assert(res.getRemaining() == "");

	res = OneOrMore(make_shared<Char>('a')).parse("aaaaab");
	assert(res);
	assert(res.getRecognized() == "aaaaa");
	assert(res.getRemaining() == "b");

	res = OneOrMore(make_shared<Char>('a')).parse("baaaaa");
	assert(!res);

	res = OneOrMore(make_shared<Char>('a')).parse("");
	assert(!res);
}

void byx::ParserTest::CallbackOnSuccessTest()
{
	bool called = false;
	ParseResult res = CallbackOnSuccess(make_shared<Char>('a'), 
		[&called](ParseResult res)
		{
			assert(res);
			assert(res.getRecognized() == "a");
			assert(res.getRemaining() == "bc");
			called = true;
		}).parse("abc");
	assert(called);

	called = false;
	res = CallbackOnSuccess(make_shared<Char>('a'), 
		[&called](ParseResult res)
		{
			assert(0);
			called = true;
		}).parse("123");
	assert(!called);
}

void byx::ParserTest::CallbackOnFailTest()
{
	bool called = false;
	ParseResult res = CallbackOnFail(make_shared<Char>('a'), 
		[&called](ParseResult res)
		{
			assert(0);
			called = true;
		}).parse("abc");
	assert(!called);

	called = false;
	res = CallbackOnFail(make_shared<Char>('a'), 
		[&called](ParseResult res)
		{
			assert(!res);
			called = true;
		}).parse("123");
	assert(called);
}

void byx::ParserTest::LazyTest()
{
	bool called = false;
	auto getParser = [&called]()
	{
		called = true;
		return make_shared<Char>('a');
	};

	shared_ptr<Lazy> lazy = make_shared<Lazy>(getParser);
	assert(!called);

	ParseResult res = lazy->parse("abc");
	assert(called);
	assert(res);
	assert(res.getRecognized() == "a");
	assert(res.getRemaining() == "bc");
}

void byx::ParserTest::SimpleExpressionEvaluatorTest()
{
	assert(SimpleExpressionEvaluator::Eval("1") == 1);
	assert(SimpleExpressionEvaluator::Eval("25") == 25);
	assert(SimpleExpressionEvaluator::Eval("1+2") == 3);
	assert(SimpleExpressionEvaluator::Eval("15-13") == 2);
	assert(SimpleExpressionEvaluator::Eval("(1+2)*(3+4)") == 21);
	assert(SimpleExpressionEvaluator::Eval("2+15*13") == 197);
	assert(SimpleExpressionEvaluator::Eval("25-(3+4*7/6*356)-22*(34+(54-6*7))+78/3") == -2388);
}

void byx::ParserTest::SimpleLexerTest()
{
	vector<pair<string, bool>> tokens = SimpleLexer::Lex("  123	56.75 \t8\n 0.23 	\n986\n 787\n 234.3432 \n\r 1.1");
	assert(tokens[0].first == "123");
	assert(tokens[0].second);
	assert(tokens[1].first == "56.75");
	assert(!tokens[1].second);
	assert(tokens[2].first == "8");
	assert(tokens[2].second);
	assert(tokens[3].first == "0.23");
	assert(!tokens[3].second);
	assert(tokens[4].first == "986");
	assert(tokens[4].second);
	assert(tokens[5].first == "787");
	assert(tokens[5].second);
	assert(tokens[6].first == "234.3432");
	assert(!tokens[6].second);
	assert(tokens[7].first == "1.1");
	assert(!tokens[7].second);
}

// SimpleLexer

vector<pair<string, bool>> ParserTest::SimpleLexer::tokens;
Rule ParserTest::SimpleLexer::blank = Rule::CharSet(" \t\n\r");
Rule ParserTest::SimpleLexer::dot = Rule::Char('.');
Rule ParserTest::SimpleLexer::digit = Rule::Range('0', '9');
Rule ParserTest::SimpleLexer::integer = digit.oneOrMore().setCallback(addInteger);
Rule ParserTest::SimpleLexer::decimal = (digit.oneOrMore() + dot + digit.oneOrMore()).setCallback(addDecimal);
Rule ParserTest::SimpleLexer::rule = (blank | decimal | integer).oneOrMore();

std::vector<pair<string, bool>> byx::ParserTest::SimpleLexer::Lex(const std::string& input)
{
	tokens.clear();
	rule.parse(input);
	return tokens;
}

void byx::ParserTest::SimpleLexer::addInteger(ParseResult r)
{
	tokens.push_back(pair<string, bool>(r.getRecognized(), true));
}

void byx::ParserTest::SimpleLexer::addDecimal(ParseResult r)
{
	tokens.push_back(pair<string, bool>(r.getRecognized(), false));
}

// SimpleExpressionEvaluator

stack<int> ParserTest::SimpleExpressionEvaluator::s;
Rule ParserTest::SimpleExpressionEvaluator::digit = Rule::Range('0', '9');
Rule ParserTest::SimpleExpressionEvaluator::integer = digit.oneOrMore();
Rule ParserTest::SimpleExpressionEvaluator::add = Rule::Char('+');
Rule ParserTest::SimpleExpressionEvaluator::sub = Rule::Char('-');
Rule ParserTest::SimpleExpressionEvaluator::mul = Rule::Char('*');
Rule ParserTest::SimpleExpressionEvaluator::div = Rule::Char('/');
Rule ParserTest::SimpleExpressionEvaluator::lp = Rule::Char('(');
Rule ParserTest::SimpleExpressionEvaluator::rp = Rule::Char(')');
Rule ParserTest::SimpleExpressionEvaluator::lazyExpr = Rule::Lazy(LazyExpr);
Rule ParserTest::SimpleExpressionEvaluator::factor = integer.setCallback(PushInt)
                                                   | lp + lazyExpr + rp;
Rule ParserTest::SimpleExpressionEvaluator::term = factor + ((mul | div) + factor).setCallback(Calc).zeroOrMore();
Rule ParserTest::SimpleExpressionEvaluator::expr = term + ((add | sub) + term).setCallback(Calc).zeroOrMore();

int byx::ParserTest::SimpleExpressionEvaluator::Eval(const string& input)
{
	while (!s.empty())	s.pop();
	expr.parse(input);
	return s.top();
}

Rule byx::ParserTest::SimpleExpressionEvaluator::LazyExpr()
{
	return expr;
}

void byx::ParserTest::SimpleExpressionEvaluator::PushInt(ParseResult r)
{
	int val;
	(stringstream() << r.getRecognized()) >> val;
	s.push(val);
}

void byx::ParserTest::SimpleExpressionEvaluator::Calc(ParseResult r)
{
	int rhs = s.top();
	s.pop();
	int lhs = s.top();
	s.pop();
	char op = r.getRecognized()[0];
	switch (op)
	{
	case '+':
		s.push(lhs + rhs);
		break;
	case '-':
		s.push(lhs - rhs);
		break;
	case '*':
		s.push(lhs * rhs);
		break;
	case '/':
		s.push(lhs / rhs);
		break;
	default:
		break;
	}
}
