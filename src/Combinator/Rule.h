#pragma once

#include "ParserCombinator.h"

namespace byx
{
	// Rule是Parser的包装类，封装了常见的Parser创建方法和操作
	// 以及重载了一些运算符，用于Parser之间的组合
	class Rule
	{
	public:

		// 单个字符
		static Rule Char(char c);

		// 字符集合
		static Rule CharSet(const std::string& chs);

		// 排除字符集合
		static Rule Exclude(const std::string& chs);

		// 字符范围
		static Rule Range(char c1, char c2);

		// 前缀
		static Rule Prefix(const std::string& pre);

		// 前缀集合
		static Rule PrefixSet(const std::vector<std::string>& pres);

		// 惰性解析
		static Rule Lazy(const std::function<Rule(void)>& getRule);
		
		// 零个或多个
		Rule zeroOrMore() const;

		// 一个或多个
		Rule oneOrMore() const;

		// 设置解析成功时回调函数
		Rule setCallback(const std::function<void(ParseResult)>& callback) const;

		// 设置解析失败时回调函数
		Rule setCallbackOnFail(const std::function<void(ParseResult)>& callback) const;

		// 连接
		Rule operator+(const Rule& rule) const;

		// 或
		Rule operator|(const Rule& rule) const;

		// 解析输入
		ParseResult parse(const std::string& input);
	private:
		std::shared_ptr<Parser> p;
		Rule(const std::shared_ptr<Parser>& p);
	};
}