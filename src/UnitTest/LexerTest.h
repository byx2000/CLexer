#pragma once

namespace byx
{
	// 词法分析器单元测试
	class LexerTest
	{
	public:
		void static Run();

	private:
		void static Integer();
		void static Decimal();
		void static IntegerDecimal();
		void static Identifier();
		void static IntegerDecimalIdent();
		void static Keyword();
		void static Add();
		void static Sub();
		void static Mul();
		void static Div();
		void static LineComment();
		void static BlockComment();
		void static Rem();
		void static Equal();
		void static LogicAnd();
		void static LogicOr();
		void static BitXor();
		void static LogicNot();
		void static Less();
		void static Larger();
		void static String();
		void static Char();
		void static Bracket();
		void static Mark();

		void static Preprocess();
		void static ArithExpr();
		void static BitExpr();
		void static CmpExpr();
		void static DeclareStmt();
	};
}