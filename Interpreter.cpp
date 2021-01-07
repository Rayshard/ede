#include "pch.h"
#include "Interpreter.h"

namespace ede::interpreter
{
	Result EvaluateExpression(Expression* _expr)
	{
		switch (_expr->GetID())
		{
			case ExprID::LITERAL:
			{
				Literal* literal = (Literal*)_expr;

				return std::visit(overloaded{
					[](UNIT _val) { return Result(_val); },
					[](INT _val) { return Result(_val); },
					[](FLOAT _val) { return Result(_val); },
					}, literal->GetValue());
			} break;
			case ExprID::BINOP:
			{
				Binop* binop = (Binop*)_expr;
				Result left = EvaluateExpression(binop->GetLeft()), right = EvaluateExpression(binop->GetRight());

				switch (binop->GetOP())
				{
					case BinopOP::ADD:
					{
						return INT(0);
					} break;
					case BinopOP::SUB:
					{
						return INT(0);
					} break;
					case BinopOP::MUL:
					{
						return INT(0);
					} break;
					case BinopOP::DIV:
					{
						return INT(0);
					} break;
					case BinopOP::MOD:
					{
						return INT(0);
					} break;
				}
			} break;
		}

		return UNIT();
	}

	Result Evaluate(Node* _node)
	{
		switch (_node->GetID())
		{
			case NodeID::STMT:
			{
				Statement* stmt = (Statement*)_node;

				switch (stmt->GetID())
				{
					case StmtID::EXPR: return EvaluateExpression((Expression*)stmt);
					default: return UNIT();
				}
			} break;
			default: return UNIT();
		}

		return UNIT();
	}
};
