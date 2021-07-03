#pragma once

#include "ParseResult.h"

#include <vector>
#include <memory>
#include <functional>

// 正规式匹配引擎
//
// 解析器用于判断输入字符串是否满足“一定的规则”
// 当输入字符串满足“一定的规则”时，解析器将消耗部分输入，然后生成“已解析部分”和“剩余部分”，并将其作为结果返回（参见ParseResult类）
// 若输入字符串不满足指定规则，则返回失败结果
// 该文件实现了若干“基本”解析器，实现了正规式中的原语操作（元字符、连接、选择、重复等）
// 这些“基本”解析器可以组合起来，从而形成更复杂的解析规则
// 也可以继承Parser解析器基类，手动实现自定义解析规则
//
// 由于解析器类族使用起来较为复杂，因此创建了Rule作为包装类以方便使用（参见Rule类）

namespace byx
{
	// 解析器基类
	class Parser
	{
	public:
		virtual ParseResult parse(const std::string& input) = 0;
		virtual ~Parser();
	};

	// 空解析器：不对输入做任何处理
	class Empty : public Parser
	{
	public:
		virtual ParseResult parse(const std::string& input) override;
	};

	// 解析任意字符：消耗输入串的第一个字符，返回成功
	class Any : public Parser
	{
	public:
		virtual ParseResult parse(const std::string& input) override;
	};

	// 解析指定字符：若输入串第一个字符等于指定字符，则消耗该字符，并返回成功，否则返回失败
	class Char : public Parser
	{
	public:
		Char(char ch);
		virtual ParseResult parse(const std::string& input) override;
	private:
		char ch;
	};

	// 解析字符集合：若输入串第一个字符在给定的字符集合中，则消耗该字符，并返回成功，否则返回失败
	class CharSet : public Parser
	{
	public:
		CharSet(const std::string& chs);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::string chs;
	};

	// 排除字符集合：若输入串第一个字符不在给定的字符集合中，则消耗该字符，并返回成功，否则返回失败
	class Exclude : public Parser
	{
	public:
		Exclude(const std::string& chs);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::string chs;
	};

	// 解析字符范围：若输入串第一个字符在给定的字符范围内，则消耗该字符，并返回成功，否则返回失败
	class Range : public Parser
	{
	public:
		Range(char c1, char c2);
		virtual ParseResult parse(const std::string& input) override;
	private:
		char c1, c2;
	};

	// 解析前缀：若输入串的前缀为指定字符串，则消耗该前缀，并返回成功，否则返回失败
	class Prefix : public Parser
	{
	public:
		Prefix(const std::string& pre);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::string pre;
	};

	// 连接：给定一组解析器，对输入串依次应用每一个解析器
	// 只有所有解析器都解析成功时，才返回成功
	class Concat : public Parser
	{
	public:
		Concat(const std::vector<std::shared_ptr<Parser>>& parsers);
		Concat(const std::initializer_list<std::shared_ptr<Parser>>& parsers);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::vector<std::shared_ptr<Parser>> parsers;
	};

	// 选择：给定一组解析器，对输入串依次应用每一个解析器
	// 只要有一个解析器解析成功，则返回成功
	// 若所有解析器都失败，则返回失败
	class ChooseOne : public Parser
	{
	public:
		ChooseOne(const std::vector<std::shared_ptr<Parser>>& parsers);
		ChooseOne(const std::initializer_list<std::shared_ptr<Parser>>& parsers);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::vector<std::shared_ptr<Parser>> parsers;
	};

	// 零个或多个：给定一个解析器，尝试对输入串多次应用指定解析器
	class ZeroOrMore : public Parser
	{
	public:
		ZeroOrMore(const std::shared_ptr<Parser>& parser);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
	};

	// 一个或多个：给定一个解析器，尝试对输入串应用指定解析器至少一次
	class OneOrMore : public Parser
	{
	public:
		OneOrMore(const std::shared_ptr<Parser>& parser);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
	};

	// 解析前缀集合：若输入串的前缀在指定集合内，则消耗该前缀，并返回成功，否则返回失败
	class PrefixSet : public Parser
	{
	public:
		PrefixSet(const std::vector<std::string>& pres);
		PrefixSet(const std::initializer_list<std::string>& pres);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::vector<std::string> pres;
	};

	// 解析成功后调用回调函数
	class CallbackOnSuccess : public Parser
	{
	public:
		CallbackOnSuccess(const std::shared_ptr<Parser>& parser, const std::function<void(ParseResult)>& callback);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
		std::function<void(ParseResult)> callback;
	};

	// 解析失败后调用回调函数
	class CallbackOnFail : public Parser
	{
	public:
		CallbackOnFail(const std::shared_ptr<Parser>& parser, const std::function<void(ParseResult)>& callback);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
		std::function<void(ParseResult)> callback;
	};

	// 惰性解析器：调用parse时才获取解析器
	class Lazy : public Parser
	{
	public:
		Lazy(const std::function<std::shared_ptr<Parser>(void)> getParser);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::function<std::shared_ptr<Parser>(void)> getParser;
	};
}