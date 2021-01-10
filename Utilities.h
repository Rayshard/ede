#pragma once

namespace ede::utilities
{
	struct Position
	{
		size_t line, column;
		Position(size_t _line, size_t _col) : line(_line), column(_col) { }

		std::string ToString() { return "(" + std::to_string(line) + ", " + std::to_string(column) + ")"; }
	};

	enum class DiagnosticType {
		ERROR_IntLitOutOfRange,
		ERROR_FloatLitOutOfRange,
		ERROR_InvalidFloatLit,
		ERROR_ExpectedAtom,
		ERROR_ExpectedStmt,
		ERROR_ExpectedSemicolon,
		ERROR_ExpectedClosingParen,
		ERROR_ExpectedIdentifier,
		ERROR_ExpectedColon,
		ERROR_ExpectedTypeName,
		ERROR_ExpectedEquals,
		ERROR_ExpectedExpr,
	};

	void PushDiagnostic(DiagnosticType, Position, std::string);
	void PrintDiagnostics();

	class StringBuilder
	{
		std::string result, indent;
	public:
		void Write(const std::string& _str);
		void WriteLine(const std::string& _str);
		void Indent();
		void Dedent();

		std::string GetString() { return result; }
	};
};