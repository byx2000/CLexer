#include "LexerTest.h"
#include "../Lexer/Lexer.h"

#include <cassert>

using namespace std;
using namespace byx;

#define assert_exception(expr)	\
do								\
{								\
    bool flag = false;			\
    try							\
    {							\
        expr;					\
    }							\
    catch (...)					\
    {							\
        flag = true;				\
    }							\
    assert(flag);				\
}								\
while (0)						\

// LexerTest

void LexerTest::Run()
{
	LineComment();
	Integer();
	Decimal();
	IntegerDecimal();
	Identifier();
	IntegerDecimalIdent();
	Keyword();
	String();
	Char();
	Add();
	Sub();
	Mul();
	Div();

	BlockComment();
	Rem();
	Equal();
	LogicAnd();
	LogicOr();
	BitXor();
	LogicNot();
	Less();
	Larger();


	Bracket();
	Mark();

	Preprocess();
	ArithExpr();
	BitExpr();
	CmpExpr();
	DeclareStmt();
}

void LexerTest::Integer()
{
	Lexer lexer;
	lexer.parse(" 0 123 4 567 8	 90		");
	assert(lexer.next() == Token(TokenType::Integer, "0"));
	assert(lexer.next() == Token(TokenType::Integer, "123"));
	assert(lexer.next() == Token(TokenType::Integer, "4"));
	assert(lexer.next() == Token(TokenType::Integer, "567"));
	assert(lexer.next() == Token(TokenType::Integer, "8"));
	assert(lexer.next() == Token(TokenType::Integer, "90"));
	assert(lexer.next() == Token(TokenType::End));
	assert(lexer.next() == Token(TokenType::End));

	assert_exception(lexer.parse("@"));
}

void LexerTest::Decimal()
{
	Lexer lexer;
	lexer.parse("	0.23 7.4 6.88		312.113	 47663.376278	");
	assert(lexer.next() == Token(TokenType::Decimal, "0.23"));
	assert(lexer.next() == Token(TokenType::Decimal, "7.4"));
	assert(lexer.next() == Token(TokenType::Decimal, "6.88"));
	assert(lexer.next() == Token(TokenType::Decimal, "312.113"));
	assert(lexer.next() == Token(TokenType::Decimal, "47663.376278"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::IntegerDecimal()
{
	Lexer lexer;
	lexer.parse("12.56 33	6 0.4 88862   7623.322 2 1 34.1	 3.14 560");
	assert(lexer.next() == Token(TokenType::Decimal, "12.56"));
	assert(lexer.next() == Token(TokenType::Integer, "33"));
	assert(lexer.next() == Token(TokenType::Integer, "6"));
	assert(lexer.next() == Token(TokenType::Decimal, "0.4"));
	assert(lexer.next() == Token(TokenType::Integer, "88862"));
	assert(lexer.next() == Token(TokenType::Decimal, "7623.322"));
	assert(lexer.next() == Token(TokenType::Integer, "2"));
	assert(lexer.next() == Token(TokenType::Integer, "1"));
	assert(lexer.next() == Token(TokenType::Decimal, "34.1"));
	assert(lexer.next() == Token(TokenType::Decimal, "3.14"));
	assert(lexer.next() == Token(TokenType::Integer, "560"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Identifier()
{
	Lexer lexer;
	lexer.parse("i j k	cnt sum res		_printf ____ _32as __asdca34__2343c  __asssd  ____235435344    weiugdbjbcdkagu238jvbdjasvg237i4572vjh7hg8");
	assert(lexer.next() == Token(TokenType::Identifier, "i"));
	assert(lexer.next() == Token(TokenType::Identifier, "j"));
	assert(lexer.next() == Token(TokenType::Identifier, "k"));
	assert(lexer.next() == Token(TokenType::Identifier, "cnt"));
	assert(lexer.next() == Token(TokenType::Identifier, "sum"));
	assert(lexer.next() == Token(TokenType::Identifier, "res"));
	assert(lexer.next() == Token(TokenType::Identifier, "_printf"));
	assert(lexer.next() == Token(TokenType::Identifier, "____"));
	assert(lexer.next() == Token(TokenType::Identifier, "_32as"));
	assert(lexer.next() == Token(TokenType::Identifier, "__asdca34__2343c"));
	assert(lexer.next() == Token(TokenType::Identifier, "__asssd"));
	assert(lexer.next() == Token(TokenType::Identifier, "____235435344"));
	assert(lexer.next() == Token(TokenType::Identifier, "weiugdbjbcdkagu238jvbdjasvg237i4572vjh7hg8"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::IntegerDecimalIdent()
{
	Lexer lexer;
	lexer.parse("23 45.76 sum 3.5 7 a 88");
	assert(lexer.next() == Token(TokenType::Integer, "23"));
	assert(lexer.next() == Token(TokenType::Decimal, "45.76"));
	assert(lexer.next() == Token(TokenType::Identifier, "sum"));
	assert(lexer.next() == Token(TokenType::Decimal, "3.5"));
	assert(lexer.next() == Token(TokenType::Integer, "7"));
	assert(lexer.next() == Token(TokenType::Identifier, "a"));
	assert(lexer.next() == Token(TokenType::Integer, "88"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Keyword()
{
	Lexer lexer;
	lexer.parse("int double char const sum struct");
	assert(lexer.next() == Token(TokenType::Keyword, "int"));
	assert(lexer.next() == Token(TokenType::Keyword, "double"));
	assert(lexer.next() == Token(TokenType::Keyword, "char"));
	assert(lexer.next() == Token(TokenType::Keyword, "const"));
	assert(lexer.next() == Token(TokenType::Identifier, "sum"));
	assert(lexer.next() == Token(TokenType::Keyword, "struct"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Add()
{
	Lexer lexer;
	lexer.parse("+ ++ += ++ + += ++++ +++++ +=++");
	assert(lexer.next() == Token(TokenType::Add, "+"));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::AddAssign, "+="));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::Add, "+"));
	assert(lexer.next() == Token(TokenType::AddAssign, "+="));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::Add, "+"));
	assert(lexer.next() == Token(TokenType::AddAssign, "+="));
	assert(lexer.next() == Token(TokenType::Inc, "++"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Sub()
{
	Lexer lexer;
	lexer.parse("- -- -= -- - -= ---- ----- -=--->");
	assert(lexer.next() == Token(TokenType::Sub, "-"));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::SubAssign, "-="));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::Sub, "-"));
	assert(lexer.next() == Token(TokenType::SubAssign, "-="));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::Sub, "-"));
	assert(lexer.next() == Token(TokenType::SubAssign, "-="));
	assert(lexer.next() == Token(TokenType::Dec, "--"));
	assert(lexer.next() == Token(TokenType::PointerMember, "->"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Mul()
{
	Lexer lexer;
	lexer.parse("* *= *= * * *=*=* ****");
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::MulAssign, "*="));
	assert(lexer.next() == Token(TokenType::MulAssign, "*="));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::MulAssign, "*="));
	assert(lexer.next() == Token(TokenType::MulAssign, "*="));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Div()
{
	Lexer lexer;
	lexer.parse("/ /= /= / / /=/=/");
	assert(lexer.next() == Token(TokenType::Div, "/"));
	assert(lexer.next() == Token(TokenType::DivAssign, "/="));
	assert(lexer.next() == Token(TokenType::DivAssign, "/="));
	assert(lexer.next() == Token(TokenType::Div, "/"));
	assert(lexer.next() == Token(TokenType::Div, "/"));
	assert(lexer.next() == Token(TokenType::DivAssign, "/="));
	assert(lexer.next() == Token(TokenType::DivAssign, "/="));
	assert(lexer.next() == Token(TokenType::Div, "/"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::LineComment()
{
	Lexer lexer;
	lexer.parse("123 3.5 sum // This is line comment \n321 5.3 mus//this//is//anothercomment");
	assert(lexer.next() == Token(TokenType::Integer, "123"));
	assert(lexer.next() == Token(TokenType::Decimal, "3.5"));
	assert(lexer.next() == Token(TokenType::Identifier, "sum"));
	assert(lexer.next() == Token(TokenType::Integer, "321"));
	assert(lexer.next() == Token(TokenType::Decimal, "5.3"));
	assert(lexer.next() == Token(TokenType::Identifier, "mus"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::BlockComment()
{
	Lexer lexer;
	lexer.parse("123 3.5 sum /*This is a comment*/321 5.3 mus /*//\\\\///*\\\\*/ i j k /*dsjgvsudkvbshjb*/");
	assert(lexer.next() == Token(TokenType::Integer, "123"));
	assert(lexer.next() == Token(TokenType::Decimal, "3.5"));
	assert(lexer.next() == Token(TokenType::Identifier, "sum"));
	assert(lexer.next() == Token(TokenType::Integer, "321"));
	assert(lexer.next() == Token(TokenType::Decimal, "5.3"));
	assert(lexer.next() == Token(TokenType::Identifier, "mus"));
	assert(lexer.next() == Token(TokenType::Identifier, "i"));
	assert(lexer.next() == Token(TokenType::Identifier, "j"));
	assert(lexer.next() == Token(TokenType::Identifier, "k"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Rem()
{
	Lexer lexer;
	lexer.parse("% %= %=	% %=%%=");
	assert(lexer.next() == Token(TokenType::Rem, "%"));
	assert(lexer.next() == Token(TokenType::RemAssign, "%="));
	assert(lexer.next() == Token(TokenType::RemAssign, "%="));
	assert(lexer.next() == Token(TokenType::Rem, "%"));
	assert(lexer.next() == Token(TokenType::RemAssign, "%="));
	assert(lexer.next() == Token(TokenType::Rem, "%"));
	assert(lexer.next() == Token(TokenType::RemAssign, "%="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Equal()
{
	Lexer lexer;
	lexer.parse("= == == = =======");
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::Equal, "=="));
	assert(lexer.next() == Token(TokenType::Equal, "=="));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::Equal, "=="));
	assert(lexer.next() == Token(TokenType::Equal, "=="));
	assert(lexer.next() == Token(TokenType::Equal, "=="));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::LogicAnd()
{
	Lexer lexer;
	lexer.parse("& && &= && & & && &&&&& &=&=&&=");
	assert(lexer.next() == Token(TokenType::BitAnd, "&"));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::BitAndAssign, "&="));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::BitAnd, "&"));
	assert(lexer.next() == Token(TokenType::BitAnd, "&"));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::BitAnd, "&"));
	assert(lexer.next() == Token(TokenType::BitAndAssign, "&="));
	assert(lexer.next() == Token(TokenType::BitAndAssign, "&="));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::LogicOr()
{
	Lexer lexer;
	lexer.parse("| || |= || | | || ||||| |=|=||=");
	assert(lexer.next() == Token(TokenType::BitOr, "|"));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::BitOrAssign, "|="));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::BitOr, "|"));
	assert(lexer.next() == Token(TokenType::BitOr, "|"));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::BitOr, "|"));
	assert(lexer.next() == Token(TokenType::BitOrAssign, "|="));
	assert(lexer.next() == Token(TokenType::BitOrAssign, "|="));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::BitXor()
{
	Lexer lexer;
	lexer.parse("^ ^= ^^ ^=^^=");
	assert(lexer.next() == Token(TokenType::BitXor, "^"));
	assert(lexer.next() == Token(TokenType::BitXorAssign, "^="));
	assert(lexer.next() == Token(TokenType::BitXor, "^"));
	assert(lexer.next() == Token(TokenType::BitXor, "^"));
	assert(lexer.next() == Token(TokenType::BitXorAssign, "^="));
	assert(lexer.next() == Token(TokenType::BitXor, "^"));
	assert(lexer.next() == Token(TokenType::BitXorAssign, "^="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::LogicNot()
{
	Lexer lexer;
	lexer.parse("! != !!! !=!==");
	assert(lexer.next() == Token(TokenType::LogicNot, "!"));
	assert(lexer.next() == Token(TokenType::NotEqual, "!="));
	assert(lexer.next() == Token(TokenType::LogicNot, "!"));
	assert(lexer.next() == Token(TokenType::LogicNot, "!"));
	assert(lexer.next() == Token(TokenType::LogicNot, "!"));
	assert(lexer.next() == Token(TokenType::NotEqual, "!="));
	assert(lexer.next() == Token(TokenType::NotEqual, "!="));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Less()
{
	Lexer lexer;
	lexer.parse("< <= << <<= <<<<=<< <==<=");
	assert(lexer.next() == Token(TokenType::Less, "<"));
	assert(lexer.next() == Token(TokenType::LessEqual, "<="));
	assert(lexer.next() == Token(TokenType::LeftShift, "<<"));
	assert(lexer.next() == Token(TokenType::LeftShiftAssign, "<<="));
	assert(lexer.next() == Token(TokenType::LeftShift, "<<"));
	assert(lexer.next() == Token(TokenType::LeftShiftAssign, "<<="));
	assert(lexer.next() == Token(TokenType::LeftShift, "<<"));
	assert(lexer.next() == Token(TokenType::LessEqual, "<="));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::LessEqual, "<="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Larger()
{
	Lexer lexer;
	lexer.parse("> >= >> >>= >>>>=>> >==>=");
	assert(lexer.next() == Token(TokenType::Larger, ">"));
	assert(lexer.next() == Token(TokenType::LargerEqual, ">="));
	assert(lexer.next() == Token(TokenType::RightShift, ">>"));
	assert(lexer.next() == Token(TokenType::RightShiftAssign, ">>="));
	assert(lexer.next() == Token(TokenType::RightShift, ">>"));
	assert(lexer.next() == Token(TokenType::RightShiftAssign, ">>="));
	assert(lexer.next() == Token(TokenType::RightShift, ">>"));
	assert(lexer.next() == Token(TokenType::LargerEqual, ">="));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::LargerEqual, ">="));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::String()
{
	Lexer lexer;
	lexer.parse("\"This is a string\"  \"\"");
	assert(lexer.next() == Token(TokenType::String, "This is a string"));
	assert(lexer.next() == Token(TokenType::String, ""));
	assert(lexer.next() == Token(TokenType::End));

	assert_exception(lexer.parse("\"jdsgfjwbgbjkgj dsfksbjk csdj"));
}

void LexerTest::Char()
{
	Lexer lexer;
	lexer.parse("'a' 'b' 'c'");
	assert(lexer.next() == Token(TokenType::Char, "a"));
	assert(lexer.next() == Token(TokenType::Char, "b"));
	assert(lexer.next() == Token(TokenType::Char, "c"));
	assert(lexer.next() == Token(TokenType::End));

	assert_exception(lexer.parse("'abc'"));
	assert_exception(lexer.parse("'adfss"));
	assert_exception(lexer.parse("'a"));
	assert_exception(lexer.parse("''"));
	assert_exception(lexer.parse("'"));
}

void LexerTest::Bracket()
{
	Lexer lexer;
	lexer.parse("()[]{}( {]{)}]");
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::OpenSquareBracket, "["));
	assert(lexer.next() == Token(TokenType::CloseSquareBracket, "]"));
	assert(lexer.next() == Token(TokenType::OpenBrace, "{"));
	assert(lexer.next() == Token(TokenType::CloseBrace, "}"));
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::OpenBrace, "{"));
	assert(lexer.next() == Token(TokenType::CloseSquareBracket, "]"));
	assert(lexer.next() == Token(TokenType::OpenBrace, "{"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::CloseBrace, "}"));
	assert(lexer.next() == Token(TokenType::CloseSquareBracket, "]"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Mark()
{
	Lexer lexer;
	lexer.parse(",,.;,,;.. ;,,;");
	assert(lexer.next() == Token(TokenType::Comma, ","));
	assert(lexer.next() == Token(TokenType::Comma, ","));
	assert(lexer.next() == Token(TokenType::Member, "."));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::Comma, ","));
	assert(lexer.next() == Token(TokenType::Comma, ","));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::Member, "."));
	assert(lexer.next() == Token(TokenType::Member, "."));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::Comma, ","));
	assert(lexer.next() == Token(TokenType::Comma, ","));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::Preprocess()
{
	Lexer lexer;
	lexer.parse("#include <stdio.h>\n #define PI 3.14\n#if");
	assert(lexer.next() == Token(TokenType::Preprocess, "#include <stdio.h>"));
	assert(lexer.next() == Token(TokenType::Preprocess, "#define PI 3.14"));
	assert(lexer.next() == Token(TokenType::Preprocess, "#if"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::ArithExpr()
{
	Lexer lexer;
	lexer.parse("1+2.34-5.6+(-367)*71.52/0.24%32");
	assert(lexer.next() == Token(TokenType::Integer, "1"));
	assert(lexer.next() == Token(TokenType::Add, "+"));
	assert(lexer.next() == Token(TokenType::Decimal, "2.34"));
	assert(lexer.next() == Token(TokenType::Sub, "-"));
	assert(lexer.next() == Token(TokenType::Decimal, "5.6"));
	assert(lexer.next() == Token(TokenType::Add, "+"));
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::Sub, "-"));
	assert(lexer.next() == Token(TokenType::Integer, "367"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Decimal, "71.52"));
	assert(lexer.next() == Token(TokenType::Div, "/"));
	assert(lexer.next() == Token(TokenType::Decimal, "0.24"));
	assert(lexer.next() == Token(TokenType::Rem, "%"));
	assert(lexer.next() == Token(TokenType::Integer, "32"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::BitExpr()
{
	Lexer lexer;
	lexer.parse("1&21^(23|54) | ~(234^(432&0))");
	assert(lexer.next() == Token(TokenType::Integer, "1"));
	assert(lexer.next() == Token(TokenType::BitAnd, "&"));
	assert(lexer.next() == Token(TokenType::Integer, "21"));
	assert(lexer.next() == Token(TokenType::BitXor, "^"));
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::Integer, "23"));
	assert(lexer.next() == Token(TokenType::BitOr, "|"));
	assert(lexer.next() == Token(TokenType::Integer, "54"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::BitOr, "|"));
	assert(lexer.next() == Token(TokenType::BitReverse, "~"));
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::Integer, "234"));
	assert(lexer.next() == Token(TokenType::BitXor, "^"));
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::Integer, "432"));
	assert(lexer.next() == Token(TokenType::BitAnd, "&"));
	assert(lexer.next() == Token(TokenType::Integer, "0"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::CmpExpr()
{
	Lexer lexer;
	lexer.parse("(3.14 > 54) && 6213<4432 || !(5.6 >= 231<=47.34) == 0");
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::Decimal, "3.14"));
	assert(lexer.next() == Token(TokenType::Larger, ">"));
	assert(lexer.next() == Token(TokenType::Integer, "54"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::LogicAnd, "&&"));
	assert(lexer.next() == Token(TokenType::Integer, "6213"));
	assert(lexer.next() == Token(TokenType::Less, "<"));
	assert(lexer.next() == Token(TokenType::Integer, "4432"));
	assert(lexer.next() == Token(TokenType::LogicOr, "||"));
	assert(lexer.next() == Token(TokenType::LogicNot, "!"));
	assert(lexer.next() == Token(TokenType::OpenParenthese, "("));
	assert(lexer.next() == Token(TokenType::Decimal, "5.6"));
	assert(lexer.next() == Token(TokenType::LargerEqual, ">="));
	assert(lexer.next() == Token(TokenType::Integer, "231"));
	assert(lexer.next() == Token(TokenType::LessEqual, "<="));
	assert(lexer.next() == Token(TokenType::Decimal, "47.34"));
	assert(lexer.next() == Token(TokenType::CloseParenthese, ")"));
	assert(lexer.next() == Token(TokenType::Equal, "=="));
	assert(lexer.next() == Token(TokenType::Integer, "0"));
	assert(lexer.next() == Token(TokenType::End));
}

void LexerTest::DeclareStmt()
{
	Lexer lexer;
	lexer.parse("int i = 100;");
	assert(lexer.next() == Token(TokenType::Keyword, "int"));
	assert(lexer.next() == Token(TokenType::Identifier, "i"));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::Integer, "100"));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::End));

	lexer.parse("double sum = 3.14 * 2;");
	assert(lexer.next() == Token(TokenType::Keyword, "double"));
	assert(lexer.next() == Token(TokenType::Identifier, "sum"));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::Decimal, "3.14"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Integer, "2"));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::End));

	lexer.parse("char ch = 'a';");
	assert(lexer.next() == Token(TokenType::Keyword, "char"));
	assert(lexer.next() == Token(TokenType::Identifier, "ch"));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::Char, "a"));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::End));

	lexer.parse("const char* str = \"hello!\";");
	assert(lexer.next() == Token(TokenType::Keyword, "const"));
	assert(lexer.next() == Token(TokenType::Keyword, "char"));
	assert(lexer.next() == Token(TokenType::Mul, "*"));
	assert(lexer.next() == Token(TokenType::Identifier, "str"));
	assert(lexer.next() == Token(TokenType::Assign, "="));
	assert(lexer.next() == Token(TokenType::String, "hello!"));
	assert(lexer.next() == Token(TokenType::Semicolon, ";"));
	assert(lexer.next() == Token(TokenType::End));
}
