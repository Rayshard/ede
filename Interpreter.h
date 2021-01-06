#pragma once

#include "ast.h"

using namespace ede::ast;

namespace ede::interpreter
{
	typedef std::variant<UNIT, INT, FLOAT> Result;

	Result Evaluate(Node* _node);
};