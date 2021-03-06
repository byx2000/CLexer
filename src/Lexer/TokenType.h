#pragma once

namespace byx
{
    // ????????
    enum class TokenType
    {
        Unknown,
        Integer,
        Decimal,
        String,
        Char,
        Identifier,
        Keyword,
        Add,
        Inc,
        AddAssign,
        Assign,
        Equal,
        OpenParenthese,
        CloseParenthese,
        OpenBrace,
        CloseBrace,
        OpenSquareBracket,
        CloseSquareBracket,
        Comma,
        Semicolon,
        Sub,
        Dec,
        SubAssign,
        Mul,
        MulAssign,
        Div,
        DivAssign,
        Rem,
        RemAssign,
        LogicNot,
        NotEqual,
        BitReverse,
        Less,
        Larger,
        LessEqual,
        LargerEqual,
        BitAnd,
        LogicAnd,
        BitAndAssign,
        BitOr,
        LogicOr,
        BitOrAssign,
        BitXor,
        BitXorAssign,
        LeftShift,
        LeftShiftAssign,
        RightShift,
        RightShiftAssign,
        Preprocess,
        Member,
        Colon,
        Question,
        PointerMember,
        End,
    };
}