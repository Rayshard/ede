#include "pch.h"
#include "Utilities.h"
#include "Lexer.h"

using namespace ede::utilities;
using namespace ede::lexer;

int main()
{
	std::ifstream file("Examples\\ex1.ede");
	Lexer lexer(4);

	auto tokens = lexer.Tokenize(file);

	for (auto tok : tokens)
		std::cout << tok.ToString() << std::endl;

	file.close();
	PrintDiagnostics();

	return 0;
}
