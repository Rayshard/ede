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
				case DiagnosticType::ERROR_ExpectedIdentifier: header += "<ERROR> Expected an identifier"; break;
				case DiagnosticType::ERROR_ExpectedColon: header += "<ERROR> Expected a colon"; break;
				case DiagnosticType::ERROR_ExpectedTypeName: header += "<ERROR> Expected a type name"; break;
				case DiagnosticType::ERROR_ExpectedEquals: header += "<ERROR> Expected an equals symbol"; break;
				case DiagnosticType::ERROR_ExpectedExpr: header += "<ERROR> Expected an expression"; break;
				default: header += "Unknown Diagnostic"; break;
			}

			std::cout << header << " : " << msg << std::endl;
		}
	}
	
	void StringBuilder::Write(const std::string& _str) { result += _str; }
	void StringBuilder::WriteLine(const std::string& _str) { result += '\n' + indent + "|-" + _str; }
	void StringBuilder::Indent() { indent.push_back(' '); }
	void StringBuilder::Dedent() { indent.pop_back(); }
};
