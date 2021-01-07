#include "pch.h"
#include "ast.h"

namespace ede::ast
{
	Type* Literal::GetType()
	{
		return new PrimitiveType(PrimitiveID::UNIT);
	}
}

