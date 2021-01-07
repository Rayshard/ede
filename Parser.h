#pragma once

#include "AST.h"

using namespace ede::ast;

namespace ede::parser
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
		std::stringstream stream;
		Position position;
		size_t tabsize;
	public:
		Lexer(const std::string& _src, size_t _tabsize) : stream(_src), position(1, 1), tabsize(_tabsize) { }

		int Peek();
		int Read();

		Position GetPosition() { return position; }
	};

	std::vector<Token> Tokenize(const std::string& _src, size_t _tabsize);
	Node* Parse(const std::string& _src, size_t _tabsize);
};