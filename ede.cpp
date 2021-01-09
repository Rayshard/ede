#include "pch.h"
#include "Utilities.h"
#include "Parser.h"

using namespace ede;
using namespace ede::utilities;

int main()
{
	std::ifstream file("Examples\\ex1.ede");

	auto node = parser::Parse(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()), 4);

	StringBuilder sb;
	node->ToString(sb);

	std::cout << sb.GetString() << std::endl;

	delete node;

	file.close();
	PrintDiagnostics();
	return 0;
}
