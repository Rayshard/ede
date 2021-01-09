#include "pch.h"
#include "ast.h"

namespace ede::ast
{
	std::string BinopOPToString(BinopOP _op)
	{
		switch (_op)
		{
			case BinopOP::ADD: return "+";
			case BinopOP::SUB: return "-";
			case BinopOP::MUL: return "*";
			case BinopOP::DIV: return "\\";
			case BinopOP::MOD: return "%";
			default: return "Unknown Binop Op";
		}
	}

	void Binop::ToString(StringBuilder& _builder)
	{
		_builder.WriteLine("Binop");
		_builder.Indent();

		_builder.WriteLine("OP: " + BinopOPToString(op));

		_builder.WriteLine("Left");
		_builder.Indent();
		left->ToString(_builder);
		_builder.Dedent();

		_builder.Write("Right");
		_builder.Indent();
		right->ToString(_builder);
		_builder.Dedent();

		_builder.Dedent();
	}
	
	void Literal::ToString(StringBuilder& _builder)
	{
		std::string valueStr = std::visit(overloaded{
			[](UNIT _val) { return std::string("()"); },
			[](INT _val) { return std::to_string(_val); },
			[](FLOAT _val) { return std::to_string(_val); },
			[](BOOL _val) { return std::to_string(_val); },
			}, value);

		_builder.WriteLine("Literal: " + valueStr);
	}
}

