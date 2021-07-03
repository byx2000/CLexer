#pragma once

#include <string>

namespace byx
{
	// 解析结果
	class ParseResult
	{
	public:
		// 获取已识别部分
		std::string getRecognized() const;
		// 获取未识别部分
		std::string  getRemaining() const;
		// 获取是否解析成功
		bool isSucceeded() const;
		// 重载bool类型转换
		operator bool() const;
		// 创建失败结果
		static ParseResult CreateFail();
		// 创建成功结果
		static ParseResult CreateSucceed(const std::string& recognized, const std::string& remaining);
		// 转换为字符串
		std::string toString() const;
	private:
		std::string recognized;
		std::string remaining;
		bool succeeded;
		// 构造函数（私有）
		ParseResult(const std::string& recognized, const std::string& remaining, bool succeeded);
	};
}