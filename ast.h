#pragma once
#include "Utilities.h"

using namespace ede::utilities;

namespace ede::ast
{
	typedef std::monostate UNIT;
	typedef long long INT;
	typedef double FLOAT;

	enum class NodeType { STMT, EXPR };
	enum class ExprType { LITERAL, BINOP };
	enum class BinopType { ADD, SUB, MUL, DIV, MOD };

	class Node
	{
		NodeType type;
		Position position;
	protected:
		Node(NodeType _type, Position _pos) : type(_type), position(_pos) { }
	public:
		NodeType GetType() { return type; }
		Position GetPosition() { return position; }
	};

	class Expression : public Node
	{
		ExprType type;
	protected:
		Expression(ExprType _type, Position _pos) : Node(NodeType::EXPR, _pos), type(_type) { }
	public:
		ExprType GetType() { return type; }
	};

	class Binop : public Expression
	{
		BinopType type;
		Expression* left, * right;
	public:
		Binop(Expression* _left, BinopType _type, Expression* _right, Position _pos) : Expression(ExprType::BINOP, _pos), left(_left), right(_right), type(_type) { }

		BinopType GetType() { return type; }
		Expression* GetLeft() { return left; }
		Expression* GetRight() { return right; }
	};

	class Literal : public Expression
	{
		typedef std::variant<UNIT, INT, FLOAT> type;
		type value;
	public:
		Literal(type _val, Position _pos) : Expression(ExprType::LITERAL, _pos), value(_val) { }

		type GetValue() { return value; }
	};
};