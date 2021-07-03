#pragma once

#include "ParseResult.h"

#include <vector>
#include <memory>
#include <functional>

// ����ʽƥ������
//
// �����������ж������ַ����Ƿ����㡰һ���Ĺ���
// �������ַ������㡰һ���Ĺ���ʱ�������������Ĳ������룬Ȼ�����ɡ��ѽ������֡��͡�ʣ�ಿ�֡�����������Ϊ������أ��μ�ParseResult�ࣩ
// �������ַ���������ָ�������򷵻�ʧ�ܽ��
// ���ļ�ʵ�������ɡ���������������ʵ��������ʽ�е�ԭ�������Ԫ�ַ������ӡ�ѡ���ظ��ȣ�
// ��Щ����������������������������Ӷ��γɸ����ӵĽ�������
// Ҳ���Լ̳�Parser���������࣬�ֶ�ʵ���Զ����������
//
// ���ڽ���������ʹ��������Ϊ���ӣ���˴�����Rule��Ϊ��װ���Է���ʹ�ã��μ�Rule�ࣩ

namespace byx
{
	// ����������
	class Parser
	{
	public:
		virtual ParseResult parse(const std::string& input) = 0;
		virtual ~Parser();
	};

	// �ս������������������κδ���
	class Empty : public Parser
	{
	public:
		virtual ParseResult parse(const std::string& input) override;
	};

	// ���������ַ����������봮�ĵ�һ���ַ������سɹ�
	class Any : public Parser
	{
	public:
		virtual ParseResult parse(const std::string& input) override;
	};

	// ����ָ���ַ��������봮��һ���ַ�����ָ���ַ��������ĸ��ַ��������سɹ������򷵻�ʧ��
	class Char : public Parser
	{
	public:
		Char(char ch);
		virtual ParseResult parse(const std::string& input) override;
	private:
		char ch;
	};

	// �����ַ����ϣ������봮��һ���ַ��ڸ������ַ������У������ĸ��ַ��������سɹ������򷵻�ʧ��
	class CharSet : public Parser
	{
	public:
		CharSet(const std::string& chs);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::string chs;
	};

	// �ų��ַ����ϣ������봮��һ���ַ����ڸ������ַ������У������ĸ��ַ��������سɹ������򷵻�ʧ��
	class Exclude : public Parser
	{
	public:
		Exclude(const std::string& chs);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::string chs;
	};

	// �����ַ���Χ�������봮��һ���ַ��ڸ������ַ���Χ�ڣ������ĸ��ַ��������سɹ������򷵻�ʧ��
	class Range : public Parser
	{
	public:
		Range(char c1, char c2);
		virtual ParseResult parse(const std::string& input) override;
	private:
		char c1, c2;
	};

	// ����ǰ׺�������봮��ǰ׺Ϊָ���ַ����������ĸ�ǰ׺�������سɹ������򷵻�ʧ��
	class Prefix : public Parser
	{
	public:
		Prefix(const std::string& pre);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::string pre;
	};

	// ���ӣ�����һ��������������봮����Ӧ��ÿһ��������
	// ֻ�����н������������ɹ�ʱ���ŷ��سɹ�
	class Concat : public Parser
	{
	public:
		Concat(const std::vector<std::shared_ptr<Parser>>& parsers);
		Concat(const std::initializer_list<std::shared_ptr<Parser>>& parsers);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::vector<std::shared_ptr<Parser>> parsers;
	};

	// ѡ�񣺸���һ��������������봮����Ӧ��ÿһ��������
	// ֻҪ��һ�������������ɹ����򷵻سɹ�
	// �����н�������ʧ�ܣ��򷵻�ʧ��
	class ChooseOne : public Parser
	{
	public:
		ChooseOne(const std::vector<std::shared_ptr<Parser>>& parsers);
		ChooseOne(const std::initializer_list<std::shared_ptr<Parser>>& parsers);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::vector<std::shared_ptr<Parser>> parsers;
	};

	// �������������һ�������������Զ����봮���Ӧ��ָ��������
	class ZeroOrMore : public Parser
	{
	public:
		ZeroOrMore(const std::shared_ptr<Parser>& parser);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
	};

	// һ������������һ�������������Զ����봮Ӧ��ָ������������һ��
	class OneOrMore : public Parser
	{
	public:
		OneOrMore(const std::shared_ptr<Parser>& parser);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
	};

	// ����ǰ׺���ϣ������봮��ǰ׺��ָ�������ڣ������ĸ�ǰ׺�������سɹ������򷵻�ʧ��
	class PrefixSet : public Parser
	{
	public:
		PrefixSet(const std::vector<std::string>& pres);
		PrefixSet(const std::initializer_list<std::string>& pres);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::vector<std::string> pres;
	};

	// �����ɹ�����ûص�����
	class CallbackOnSuccess : public Parser
	{
	public:
		CallbackOnSuccess(const std::shared_ptr<Parser>& parser, const std::function<void(ParseResult)>& callback);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
		std::function<void(ParseResult)> callback;
	};

	// ����ʧ�ܺ���ûص�����
	class CallbackOnFail : public Parser
	{
	public:
		CallbackOnFail(const std::shared_ptr<Parser>& parser, const std::function<void(ParseResult)>& callback);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::shared_ptr<Parser> parser;
		std::function<void(ParseResult)> callback;
	};

	// ���Խ�����������parseʱ�Ż�ȡ������
	class Lazy : public Parser
	{
	public:
		Lazy(const std::function<std::shared_ptr<Parser>(void)> getParser);
		virtual ParseResult parse(const std::string& input) override;
	private:
		std::function<std::shared_ptr<Parser>(void)> getParser;
	};
}