#pragma once

#include "ParserCombinator.h"

namespace byx
{
	// Rule��Parser�İ�װ�࣬��װ�˳�����Parser���������Ͳ���
	// �Լ�������һЩ�����������Parser֮������
	class Rule
	{
	public:

		// �����ַ�
		static Rule Char(char c);

		// �ַ�����
		static Rule CharSet(const std::string& chs);

		// �ų��ַ�����
		static Rule Exclude(const std::string& chs);

		// �ַ���Χ
		static Rule Range(char c1, char c2);

		// ǰ׺
		static Rule Prefix(const std::string& pre);

		// ǰ׺����
		static Rule PrefixSet(const std::vector<std::string>& pres);

		// ���Խ���
		static Rule Lazy(const std::function<Rule(void)>& getRule);
		
		// �������
		Rule zeroOrMore() const;

		// һ������
		Rule oneOrMore() const;

		// ���ý����ɹ�ʱ�ص�����
		Rule setCallback(const std::function<void(ParseResult)>& callback) const;

		// ���ý���ʧ��ʱ�ص�����
		Rule setCallbackOnFail(const std::function<void(ParseResult)>& callback) const;

		// ����
		Rule operator+(const Rule& rule) const;

		// ��
		Rule operator|(const Rule& rule) const;

		// ��������
		ParseResult parse(const std::string& input);
	private:
		std::shared_ptr<Parser> p;
		Rule(const std::shared_ptr<Parser>& p);
	};
}