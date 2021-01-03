#include "pch.h"
#include <fstream>
#include <string>


struct Position
{
	size_t line, column;
	Position(size_t _line, size_t _col) : line(_line), column(_col) { }

	std::string ToString() { return "(" + std::to_string(line) + ", " + std::to_string(column) + ")"; }
};

enum class TokenID
{
	KW_LET, KW_INT,
	SYM_COLON, SYM_SEMICOLON, SYM_EQUALS,
	IDENTIFIER, LIT_INT,

	END_OF_FILE, INVALID
};

struct Token
{
	TokenID id;
	Position position;
	std::string value;

	Token(TokenID _id, Position _pos, std::string _val = "") : id(_id), position(_pos), value(_val) { }

	std::string ToString()
	{
		std::string result = position.ToString() + " ";

		switch (id)
		{
			case TokenID::KW_LET: result += "let"; break;
			case TokenID::KW_INT: result += "int"; break;
			case TokenID::SYM_COLON: result += "COLON"; break;
			case TokenID::SYM_SEMICOLON: result += "SEMICOLON"; break;
			case TokenID::SYM_EQUALS: result += "EQUALS"; break;
			case TokenID::IDENTIFIER: result += "ID: " + value; break;
			case TokenID::LIT_INT: result += "Integer Literal: " + value; break;
			case TokenID::END_OF_FILE: result += "EOF"; break;
			case TokenID::INVALID: result += "Invalid: " + value; break;
			default: result += "Unknown: " + value; break;
		}

		return result;
	}
};


class Lexer
{
	Position position;
	size_t tabsize;
public:
	Lexer(size_t _tabsize) : position(1, 1), tabsize(_tabsize) { }

	int Peek(std::ifstream& _stream) { return _stream.peek(); }

	int Read(std::ifstream& _stream)
	{
		int result = _stream.get();

		if (result == '\n')
		{
			position.line++;
			position.column = 1;
		}
		else if (result == '\t') { position.column += tabsize; }
		else { position.column++; }

		return result;
	}

	Position GetPosition() { return position; }
};

std::vector<Token> Tokenize(std::ifstream& _stream)
{
	Lexer lexer(4);
	std::vector<Token> result;

	while (true)
	{
		int peeked = lexer.Peek(_stream);

		while (std::isspace(peeked))
		{
			lexer.Read(_stream);
			peeked = lexer.Peek(_stream);
		}

		switch (peeked)
		{
			case ':': { result.push_back(Token(TokenID::SYM_COLON, lexer.GetPosition())); lexer.Read(_stream); } break;
			case ';': { result.push_back(Token(TokenID::SYM_SEMICOLON, lexer.GetPosition())); lexer.Read(_stream); } break;
			case '=': { result.push_back(Token(TokenID::SYM_EQUALS, lexer.GetPosition())); lexer.Read(_stream); } break;
			default:
			{
				if (std::isalpha(peeked) || peeked == '_') // Identifier or Keyword
				{
					std::string value;
					Position position = lexer.GetPosition();
					
					do
					{
						value += lexer.Read(_stream);
						peeked = lexer.Peek(_stream);
					} while (std::isalnum(peeked) || peeked == '_');

					static std::unordered_map<std::string, TokenID> KEYWORDS = {
						{"let", TokenID::KW_LET }, {"int", TokenID::KW_INT }
					};

					auto keywordSearch = KEYWORDS.find(value);
					if (keywordSearch != KEYWORDS.end()) { result.push_back(Token(keywordSearch->second, position)); }
					else { result.push_back(Token(TokenID::IDENTIFIER, position, value)); }
				}
				else if (std::isdigit(peeked)) // Numeric Literal
				{
					std::string value;
					Position position = lexer.GetPosition();

					do
					{
						value += lexer.Read(_stream);
						peeked = lexer.Peek(_stream);
					} while (std::isdigit(peeked));

					result.push_back(Token(TokenID::LIT_INT, position, value));
				}
				else if (peeked == EOF) // End of File
				{
					lexer.Read(_stream);
					result.push_back(Token(TokenID::END_OF_FILE, lexer.GetPosition()));
					return result;
				}
				else // Invalid
				{
					result.push_back(Token(TokenID::INVALID, lexer.GetPosition()));
					lexer.Read(_stream);
				}
			} break;
		}
	}
}

int main()
{
	std::ifstream file("Examples\\ex1.ede");

	auto tokens = Tokenize(file);

	for (auto tok : tokens)
		std::cout << tok.ToString() << std::endl;

	file.close();
	return 0;
}
