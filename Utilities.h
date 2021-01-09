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
		ERROR_ExpectedClosingParen
	};

	void PushDiagnostic(DiagnosticType, Position, std::string);
	void PrintDiagnostics();

	class StringBuilder
	{
		std::string result, indent;
	public:
		void Write(const std::string& _str) { result += _str; }
		void WriteLine(const std::string& _str) { result += _str + '\n' + indent; }
		void Indent() { indent.push_back('\t'); }
		void Dedent() { indent.pop_back(); }

		std::string GetString() { return result; }
	};
};