#pragma once
#include "Utilities.h"
#include "Typesystem.h"

using namespace ede::utilities;
using namespace ede::typesystem;

namespace ede::ast
{
	typedef std::monostate UNIT;
	typedef long long INT;
	typedef double FLOAT;

	enum class NodeID { STMT };
	enum class StmtID { EXPR };
	enum class ExprID { LITERAL, BINOP };
	enum class BinopOP { ADD, SUB, MUL, DIV, MOD };

	class Node
	{
		NodeID id;
		Position position;
	protected:
		Node(NodeID _id, Position _pos) : id(_id), position(_pos) { }
	public:
		NodeID GetID() { return id; }
		Position GetPosition() { return position; }
	};

	class Statement : public Node
	{
		StmtID id;
		Type* type;
	protected:
		Statement(StmtID _id, Position _pos) : Node(NodeID::STMT, _pos), id(_id) { }
		~Statement() { delete type; }
		
		virtual Type* _TypeCheck() { return new PrimitiveType(PrimitiveID::UNIT); }
	public:
		Type* TypeCheck() { return type ? type : (type = _TypeCheck()); }

		StmtID GetID() { return id; }
		Type* GetType() { return type; }
	};

	class Expression : public Statement
	{
		ExprID id;
	protected:
		Expression(ExprID _id, Position _pos) : Statement(StmtID::EXPR, _pos), id(_id) { }
	public:
		ExprID GetID() { return id; }
	};

	class Binop : public Expression
	{
		BinopOP op;
		Expression* left, * right;
	public:
		Binop(Expression* _left, BinopOP _op, Expression* _right, Position _pos) : Expression(ExprID::BINOP, _pos), left(_left), right(_right), op(_op) { }

		BinopOP GetOP() { return op; }
		Expression* GetLeft() { return left; }
		Expression* GetRight() { return right; }
	};

	class Literal : public Expression
	{
		typedef std::variant<UNIT, INT, FLOAT> type;
		type value;
	public:
		Literal(type _val, Position _pos) : Expression(ExprID::LITERAL, _pos), value(_val) { }

		type GetValue() { return value; }

		Type* GetType();
	};
};