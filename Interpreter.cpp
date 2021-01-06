#include "pch.h"
#include "Interpreter.h"

namespace ede::interpreter
{
	Result Evaluate(Node* _node)
	{
		switch (_node->GetType())
		{
			case NodeType::STMT: return UNIT();
			case NodeType::EXPR:
			{
				Expression* expr = (Expression*)_node;

				switch (expr->GetType())
				{
					case ExprType::LITERAL:
					{
						Literal* literal = (Literal*)expr;

						return std::visit(overloaded{
							[](UNIT _val) { return Result(_val); },
							[](INT _val) { return Result(_val); },
							[](FLOAT _val) { return Result(_val); },
							}, literal->GetValue());
					} break;
					case ExprType::BINOP:
					{
						Binop* binop = (Binop*)expr;
						Result left = Evaluate(binop->GetLeft()), right = Evaluate(binop->GetRight());

						switch (binop->GetType())
						{
							case BinopType::ADD:
							{
								return INT(0);
							} break;
							case BinopType::SUB:
							{
								return INT(0);
							} break;
							case BinopType::MUL:
							{
								return INT(0);
							} break;
							case BinopType::DIV:
							{
								return INT(0);
							} break;
							case BinopType::MOD:
							{
								return INT(0);
							} break;
						}
					} break;
				}
			};
		}

		return UNIT();
	}
};
