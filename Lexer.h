#pragma once
#include "Utilities.h"

using namespace ede::utilities;

namespace ede::lexer
{
	enum class TokenID
	{
		KW_LET, KW_INT, KW_FLOAT, KW_BOOL, KW_TRUE, KW_FALSE,
		SYM_COLON, SYM_SEMICOLON, SYM_EQUALS,
		SYM_PLUS, SYM_MINUS, SYM_ASTERISK, SYM_FSLASH, SYM_PERCENT,
		SYM_LPAREN, SYM_RPAREN,
		IDENTIFIER, LIT_INT, LIT_FLOAT,

		END_OF_FILE, INVALID
	};

	struct Token
	{
		TokenID id;
		Position position;
		std::string value;

		Token(TokenID _id, Position _pos, std::string _val = "") : id(_id), position(_pos), value(_val) { }

		std::string ToString();
	};


	class Lexer
	{
		Position position;
		size_t tabsize;
	public:
		Lexer(size_t _tabsize) : position(1, 1), tabsize(_tabsize) { }

		int Peek(std::ifstream& _stream);
		int Read(std::ifstream& _stream);
		std::vector<Token> Tokenize(std::ifstream& _stream);

		Position GetPosition() { return position; }
	};

};