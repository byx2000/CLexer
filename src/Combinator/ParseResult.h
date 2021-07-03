#pragma once

#include <string>

namespace byx
{
	// �������
	class ParseResult
	{
	public:
		// ��ȡ��ʶ�𲿷�
		std::string getRecognized() const;
		// ��ȡδʶ�𲿷�
		std::string  getRemaining() const;
		// ��ȡ�Ƿ�����ɹ�
		bool isSucceeded() const;
		// ����bool����ת��
		operator bool() const;
		// ����ʧ�ܽ��
		static ParseResult CreateFail();
		// �����ɹ����
		static ParseResult CreateSucceed(const std::string& recognized, const std::string& remaining);
		// ת��Ϊ�ַ���
		std::string toString() const;
	private:
		std::string recognized;
		std::string remaining;
		bool succeeded;
		// ���캯����˽�У�
		ParseResult(const std::string& recognized, const std::string& remaining, bool succeeded);
	};
}