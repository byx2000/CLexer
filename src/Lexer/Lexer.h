#pragma once

#include "Token.h"
#include "../Combinator/Rule.h"

#include <vector>

namespace byx
{
	// 词法分析异常类
	struct LexError
	{
		// 错误消息
		std::string message;
		LexError(const std::string& message);
	};

	// 词法分析类
	class Lexer
	{
	public:
		// 构造函数
		Lexer();
		// 词法分析
		void parse(const std::string& input);
		// 获取下一个单词（移动当前索引）
		Token next();
		// 偷看下一个单词（不移动当前索引）
		Token peek();
	private:
		// 单词列表
		std::vector<Token> tokens;
		// 当前单词索引
		size_t currentIndex;
		// 添加单词
		void addToken(TokenType type, const std::string& value = "");
		// 获取运算符解析规则
		Rule getTokenRule(const std::string& value, TokenType type);
	};
}