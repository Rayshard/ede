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
	typedef bool BOOL;

	enum class NodeID { STMT };
	enum class StmtID { EXPR, BLOCK, VARDECL };
	enum class ExprID { LITERAL, BINOP };
	enum class BinopOP { ADD, SUB, MUL, DIV, MOD };

#pragma region Node
	class Node
	{
		NodeID id;
		Position position;
	protected:
		Node(NodeID _id, Position _pos) : id(_id), position(_pos) { }
	public:
		NodeID GetID() { return id; }
		Position GetPosition() { return position; }

		virtual void ToString(StringBuilder& _builder) = 0;
	};
#pragma endregion

#pragma region Statement
	class Statement : public Node
	{
		StmtID id;
	protected:
		Statement(StmtID _id, Position _pos) : Node(NodeID::STMT, _pos), id(_id) { }
	public:
		StmtID GetID() { return id; }

		virtual void ToString(StringBuilder& _builder) = 0;
	};
#pragma endregion

#pragma region Expression
	class Expression : public Statement
	{
		ExprID id;
	protected:
		Expression(ExprID _id, Position _pos) : Statement(StmtID::EXPR, _pos), id(_id) { }
	public:
		ExprID GetID() { return id; }

		virtual void ToString(StringBuilder& _builder) = 0;
	};
#pragma endregion

#pragma region Block
	class Block : public Statement
	{
		std::vector<Statement*> statements;
	public:
		Block(std::vector<Statement*>& _stmts, Position _pos) : Statement(StmtID::BLOCK, _pos), statements(_stmts) { }
		~Block() { DELETE_VEC(statements); }

		const std::vector<Statement*>& GetStatements() { return statements; }
		
		void ToString(StringBuilder& _builder);
	};
#pragma endregion

#pragma region VarDecl
	class VarDecl : public Statement
	{
		std::string varName, typeName;
		Expression* expr;
	public:
		VarDecl(std::string _varName, std::string _typeName, Expression* _expr, Position _pos) : Statement(StmtID::VARDECL, _pos), varName(_varName), typeName(_typeName), expr(_expr) { }
		~VarDecl() { delete expr; }

		std::string GetVarName() { return varName; }
		std::string GetTypeName() { return typeName; }
		Expression* GetExpr() { return expr; }

		void ToString(StringBuilder& _builder);
	};
#pragma endregion

#pragma region Binop
	class Binop : public Expression
	{
		Expression* left, * right;
		BinopOP op;
	public:
		Binop(Expression* _left, BinopOP _op, Expression* _right, Position _pos) : Expression(ExprID::BINOP, _pos), left(_left), right(_right), op(_op) { }
		~Binop() { delete left; delete right; };

		BinopOP GetOP() { return op; }
		Expression* GetLeft() { return left; }
		Expression* GetRight() { return right; }

		void ToString(StringBuilder& _builder);
	};
#pragma endregion

#pragma region Literal
	class Literal : public Expression
	{
		typedef std::variant<UNIT, INT, FLOAT, BOOL> kind;
		kind value;
	public:
		Literal(kind _val, Position _pos) : Expression(ExprID::LITERAL, _pos), value(_val) { }

		kind GetValue() { return value; }

		void ToString(StringBuilder& _builder);
	};
#pragma endregion
};