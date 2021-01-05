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
		ERROR_IntLitOutOfRange
	};

	void PushDiagnostic(DiagnosticType, Position, std::string);
	void PrintDiagnostics();
};