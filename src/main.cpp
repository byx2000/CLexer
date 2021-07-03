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
	// �ڴ�й©���
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ��Ԫ����
	ParserTest::Run();
	LexerTest::Run();

	// �����ļ���input.c
	ifstream fin("input.c");

	// ����ļ���output.txt
	ofstream fout("output.txt");

	// �������ļ�ת�����ַ���
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