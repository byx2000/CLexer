#pragma once

#include <string>

#include "TokenType.h"

namespace byx
{
    // ����
    struct Token
    {
        TokenType type;
        std::string value;

        Token(TokenType type = TokenType::Unknown, const std::string& val = "");
        std::string toString() const;
        bool operator==(const Token& token) const;
    };
}