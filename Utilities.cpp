#include "pch.h"
#include "Utilities.h"

namespace ede::utilities
{
	typedef std::tuple<DiagnosticType, Position, std::string> Diagnostic;
	std::vector<Diagnostic> diagnostics;

	void PushDiagnostic(DiagnosticType _type, Position _pos, std::string _msg) { diagnostics.push_back(Diagnostic(_type, _pos, _msg)); }
	
	void PrintDiagnostics()
	{
		for (auto [type, pos, msg] : diagnostics)
		{
			std::string header = pos.ToString() + " ";

			switch (type)
			{
				case DiagnosticType::ERROR_IntLitOutOfRange: header += "<ERROR> Integer Literal Out Of Range"; break;
				case DiagnosticType::ERROR_FloatLitOutOfRange: header += "<ERROR> Float Literal Out Of Range"; break;
				case DiagnosticType::ERROR_InvalidFloatLit: header += "<ERROR> Invalid Floating Point Literal"; break;
				case DiagnosticType::ERROR_ExpectedAtom: header += "<ERROR> Expeected an expression"; break;
				case DiagnosticType::ERROR_ExpectedStmt: header += "<ERROR> Expected a statement"; break;
				case DiagnosticType::ERROR_ExpectedSemicolon: header += "<ERROR> Expected a semicolon"; break;
				case DiagnosticType::ERROR_ExpectedClosingParen: header += "<ERROR> Expected a closing parenthesis"; break;
				default: header += "Unknown Diagnostic"; break;
			}

			std::cout << header << " : " << msg << std::endl;
		}
	}
};
