#pragma once

#include "Token.h"
#include "../Combinator/Rule.h"

#include <vector>

namespace byx
{
	// �ʷ������쳣��
	struct LexError
	{
		// ������Ϣ
		std::string message;
		LexError(const std::string& message);
	};

	// �ʷ�������
	class Lexer
	{
	public:
		// ���캯��
		Lexer();
		// �ʷ�����
		void parse(const std::string& input);
		// ��ȡ��һ�����ʣ��ƶ���ǰ������
		Token next();
		// ͵����һ�����ʣ����ƶ���ǰ������
		Token peek();
	private:
		// �����б�
		std::vector<Token> tokens;
		// ��ǰ��������
		size_t currentIndex;
		// ��ӵ���
		void addToken(TokenType type, const std::string& value = "");
		// ��ȡ�������������
		Rule getTokenRule(const std::string& value, TokenType type);
	};
}