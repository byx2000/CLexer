#include "Token.h"

using namespace std;
using namespace byx;

static string TokenTypeString[] =
{
    "Unknown",
    "Integer",
    "Decimal",
    "String",
    "Char",
    "Identifier",
    "Keyword",
    "Add",
    "Inc",
    "AddAssign",
    "Assign",
    "Equal",
    "OpenParenthese",
    "CloseParenthese",
    "OpenBrace",
    "CloseBrace",
    "OpenSquareBracket",
    "CloseSquareBracket",
    "Comma",
    "Semicolon",
    "Sub",
    "Dec",
    "SubAssign",
    "Mul",
    "MulAssign",
    "Div",
    "DivAssign",
    "Rem",
    "RemAssign",
    "LogicNot",
    "NotEqual",
    "BitReverse",
    "Less",
    "Larger",
    "LessEqual",
    "LargerEqual",
    "BitAnd",
    "LogicAnd",
    "BitAndAssign",
    "BitOr",
    "LogicOr",
    "BitOrAssign",
    "BitXor",
    "BitXorAssign",
    "LeftShift",
    "LeftShiftAssign",
    "RightShift",
    "RightShiftAssign",
    "Preprocess",
    "Member",
    "Colon",
    "Question",
    "PointerMember",
    "End"
};

// Token

Token::Token(TokenType type, const std::string& val) : type(type),  value(val) {}

std::string Token::toString() const
{
    string s = "<";
    s += TokenTypeString[(int)type];
    if (value != "")
    {
        s += ",\"";
        s += value;
        s += "\"";
    }
    s += ">";
    return s;
}

bool Token::operator==(const Token& token) const
{
    return type == token.type && value == token.value;
}
