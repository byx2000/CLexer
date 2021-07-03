#include <iostream>
#include <stack>
#include <sstream>
#include <fstream>

#include "UnitTest/CombinatorTest.h"
#include "UnitTest/LexerTest.h"
#include "Combinator/Rule.h"
#include "Lexer/Lexer.h"

using namespace std;
using namespace byx;

int main()
{
	// 内存泄漏检测
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 单元测试
	ParserTest::Run();
	LexerTest::Run();

	// 输入文件：input.c
	ifstream fin("input.c");

	// 输出文件：output.txt
	ofstream fout("output.txt");

	// 将输入文件转换成字符串
	string input = (stringstream() << fin.rdbuf()).str();

	try
	{
		Lexer lexer;
		lexer.parse(input);

		Token token = lexer.next();
		while (token.type != TokenType::End)
		{
			fout << token.toString() << endl;
			token = lexer.next();
		}

		cout << "finish";
	}
	catch (LexError err)
	{
		cout << "error: " << endl;
		cout << err.message;
	}

	return 0;
}