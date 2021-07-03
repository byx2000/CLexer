#include "Lexer.h"

using namespace std;
using namespace byx;

// LexError

LexError::LexError(const std::string& message) : message(message) {}

//Lexer

Lexer::Lexer() : currentIndex(0) {}

void Lexer::parse(const std::string& input)
{
	// �հ׷�
	Rule blank = Rule::CharSet(" \t\n\r");

	// �������� [0-9]
	Rule digit = Rule::Range('0', '9');

	// ������ĸ [a-zA-Z]
	Rule alpha = Rule::Range('a', 'z') | Rule::Range('A', 'Z');

	// С���� \.
	Rule decimalPoint = Rule::Char('.');

	// �»��� _
	Rule underline = Rule::Char('_');

	// ��ע�� //[^\n]*\n
	Rule lineComment = Rule::Prefix("//") + Rule::Exclude("\n").zeroOrMore() + Rule::Char('\n');

	// ��ע�� /\*([^\*]|(\*[^/]))*\*/
	Rule blockComent = Rule::Prefix("/*") + (Rule::Exclude("*") | (Rule::Char('*') + Rule::Exclude("/"))).zeroOrMore() + Rule::Prefix("*/");

	// Ԥ�������#([^\n]*)\n
	Rule preProcess = (Rule::Char('#') + Rule::Exclude("\n").zeroOrMore() + Rule::Char('\n')).setCallback
	(
		[this](ParseResult r)
		{
			string value = r.getRecognized();
			addToken(TokenType::Preprocess, value.substr(0, value.size() - 1));
		}
	);

	// ������(digit)+
	Rule integer = digit.oneOrMore().setCallback
	(
		[this](ParseResult r)
		{
			addToken(TokenType::Integer, r.getRecognized());
		}
	);

	// ʵ����(digit)+(\.)(digit)+
	Rule decimal = (digit.oneOrMore() + decimalPoint + digit.oneOrMore()).setCallback
	(
		[this](ParseResult r)
		{
			addToken(TokenType::Decimal, r.getRecognized());
		}
	);

	// ��ʶ����(alpha|underline)(digit|alpha|underline)*
	Rule identifier = ((alpha | underline).oneOrMore() + (digit | alpha | underline).zeroOrMore()).setCallback
	(
		[this](ParseResult r)
		{
			addToken(TokenType::Identifier, r.getRecognized());
		}
	);

	// �ؼ���
	Rule keywords = Rule::PrefixSet
	(
		{
			"int",
			"double",
			"char",
			"long",
			"unsigned",
			"short",
			"true",
			"false",
			"void",
			"if",
			"while",
			"for",
			"do",
			"continue",
			"break",
			"switch",
			"case",
			"default",
			"const",
			"return",
			"struct",
			"static",
			"register",
			"volatile",
			"sizeof",
		}
	).setCallback
	(
		[this](ParseResult r)
		{
			addToken(TokenType::Keyword, r.getRecognized());
		}
	);

	// �ַ�����������"[^"]*"
	Rule stringLiteral = (Rule::Char('"') + Rule::Exclude("\"").zeroOrMore() + Rule::Char('"')).setCallback
	(
		[this](ParseResult r)
		{
			string value = r.getRecognized();
			string s = "";
			for (size_t i = 1; i + 1 < value.size(); ++i)
			{
				s.push_back(value[i]);
			}
			addToken(TokenType::String, s);
		}
	);

	// �ַ���������'[^']'
	Rule charLiteral = (Rule::Char('\'') + Rule::Exclude("'") + Rule::Char('\'')).setCallback
	(
		[this](ParseResult r)
		{
			string value = r.getRecognized();
			string s = "";
			s.push_back(value[1]);
			addToken(TokenType::Char, s);
		}
	);

	// �����
	Rule inc = getTokenRule("++", TokenType::Inc);
	Rule addAssign = getTokenRule("+=", TokenType::AddAssign);
	Rule add = getTokenRule("+", TokenType::Add);
	Rule dec = getTokenRule("--", TokenType::Dec);
	Rule subAssign = getTokenRule("-=", TokenType::SubAssign);
	Rule member = getTokenRule("->", TokenType::PointerMember);
	Rule sub = getTokenRule("-", TokenType::Sub);
	Rule mulAssign = getTokenRule("*=", TokenType::MulAssign);
	Rule mul = getTokenRule("*", TokenType::Mul);
	Rule divAssign = getTokenRule("/=", TokenType::DivAssign);
	Rule div = getTokenRule("/", TokenType::Div);
	Rule remAssign = getTokenRule("%=", TokenType::RemAssign);
	Rule rem = getTokenRule("%", TokenType::Rem);
	Rule equal = getTokenRule("==", TokenType::Equal);
	Rule assign = getTokenRule("=", TokenType::Assign);
	Rule bitAndAssign = getTokenRule("&=", TokenType::BitAndAssign);
	Rule logicAnd = getTokenRule("&&", TokenType::LogicAnd);
	Rule bitAnd = getTokenRule("&", TokenType::BitAnd);
	Rule bitOrAssign = getTokenRule("|=", TokenType::BitOrAssign);
	Rule logicOr = getTokenRule("||", TokenType::LogicOr);
	Rule bitOr = getTokenRule("|", TokenType::BitOr);
	Rule bitXorAssign = getTokenRule("^=", TokenType::BitXorAssign);
	Rule bitXor = getTokenRule("^", TokenType::BitXor);
	Rule notEqual = getTokenRule("!=", TokenType::NotEqual);
	Rule logicNot = getTokenRule("!", TokenType::LogicNot);
	Rule leftShiftAssign = getTokenRule("<<=", TokenType::LeftShiftAssign);
	Rule leftShift = getTokenRule("<<", TokenType::LeftShift);
	Rule lessEqual = getTokenRule("<=", TokenType::LessEqual);
	Rule less = getTokenRule("<", TokenType::Less);
	Rule rightShiftAssign = getTokenRule(">>=", TokenType::RightShiftAssign);
	Rule rightShift = getTokenRule(">>", TokenType::RightShift);
	Rule largerEqual = getTokenRule(">=", TokenType::LargerEqual);
	Rule larger = getTokenRule(">", TokenType::Larger);

	// ����
	Rule openParenthese = getTokenRule("(", TokenType::OpenParenthese);
	Rule closeParenthese = getTokenRule(")", TokenType::CloseParenthese);
	Rule openSquareBracket = getTokenRule("[", TokenType::OpenSquareBracket);
	Rule closeSquareBracket = getTokenRule("]", TokenType::CloseSquareBracket);
	Rule openBrace = getTokenRule("{", TokenType::OpenBrace);
	Rule closeBrace = getTokenRule("}", TokenType::CloseBrace);

	// ��������
	Rule comma = getTokenRule(",", TokenType::Comma);
	Rule memberPoint = getTokenRule(".", TokenType::Member);
	Rule semicolon = getTokenRule(";", TokenType::Semicolon);
	Rule colon = getTokenRule(":", TokenType::Colon);
	Rule bitNot = getTokenRule("~", TokenType::BitReverse);
	Rule question = getTokenRule("?", TokenType::Question);

	// �ܹ��� (r1|r2|r3|...)+
	Rule rules =
	(
		blank | lineComment | blockComent |
		decimal | integer | keywords | identifier | stringLiteral | charLiteral | preProcess |
		openParenthese | closeParenthese | openSquareBracket | closeSquareBracket | openBrace | closeBrace |
		inc | addAssign | add |
		dec | subAssign | member | sub |
		mulAssign | mul |
		divAssign | div |
		remAssign | rem |
		equal | assign |
		logicAnd | bitAndAssign | bitAnd |
		logicOr | bitOrAssign | bitOr |
		bitXorAssign | bitXor |
		notEqual | logicNot |
		leftShiftAssign | leftShift | lessEqual | less |
		rightShiftAssign | rightShift | largerEqual | larger |
		comma | memberPoint | semicolon | colon | bitNot | question
	).oneOrMore();

	// ����
	ParseResult res = rules.parse(input + "\n");
	if (!res || res.getRemaining() != "")
	{
		throw LexError("Lexical error.");
	}
}

Token Lexer::next()
{
	if (currentIndex == tokens.size())
	{
		return Token(TokenType::End);
	}
	return tokens[currentIndex++];
}

Token Lexer::peek()
{
	if (currentIndex == tokens.size())
	{
		return Token(TokenType::End);
	}
	return tokens[currentIndex];
}

void Lexer::addToken(TokenType type, const std::string& value)
{
	tokens.push_back(Token(type, value));
}

Rule Lexer::getTokenRule(const std::string& value, TokenType type)
{
	Rule rule = Rule::Prefix(value).setCallback
	(
		[this, type, value](ParseResult r)
		{
			addToken(type, value);
		}
	);
	return rule;
}
