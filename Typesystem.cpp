#include "pch.h"
#include "typesystem.h"

namespace ede::typesystem
{
    std::string PrimitiveType::ToString()
    {
		switch (id)
		{
			case PrimitiveID::UNIT: return "unit";
			case PrimitiveID::INT: return "int";
			case PrimitiveID::FLOAT: return "float";
		}

		return "UNKONWN";
    }
};
