#pragma once

namespace ede::typesystem
{
	enum class TypeID { PRIMITIVE };
	enum class PrimitiveID { UNIT, INT, FLOAT };

	class Type
	{
		TypeID id;
	protected:
		Type(TypeID _id) : id(_id) { }
	public:
		TypeID GetID() { return id; }

		virtual std::string ToString() = 0;
	};

	class PrimitiveType : public Type
	{
		PrimitiveID id;
	public:
		PrimitiveType(PrimitiveID _id) : Type(TypeID::PRIMITIVE), id(_id) { }
		PrimitiveID GetID() { return id; }

		std::string ToString();
	};
}