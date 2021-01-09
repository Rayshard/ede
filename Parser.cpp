#include "pch.h"
#include "Parser.h"

namespace ede::parser
{
#pragma region Lexing
	std::string Token::ToString()
	{
		std::string result = position.ToString() + " ";

		switch (id)
		{
			case TokenID::KW_LET: result += "let"; break;
			case TokenID::KW_INT: result += "int"; break;
			case TokenID::KW_FLOAT: result += "float"; break;
			case TokenID::SYM_COLON: result += "COLON"; break;
			case TokenID::SYM_SEMICOLON: result += "SEMICOLON"; break;
			case TokenID::SYM_EQUALS: result += "EQUALS"; break;
			case TokenID::SYM_PLUS: result += "PLUS"; break;
			case TokenID::SYM_MINUS: result += "MINUS"; break;
			case TokenID::SYM_ASTERISK: result += "ASTERISK"; break;
			case TokenID::SYM_FSLASH: result += "FSLASH"; break;
			case TokenID::SYM_PERCENT: result += "PERCENT"; break;
			case TokenID::SYM_LPAREN: result += "LPAREN"; break;
			case TokenID::SYM_RPAREN: result += "RPAREN"; break;
			case TokenID::IDENTIFIER: result += "ID: " + value; break;
			case TokenID::LIT_INT: result += "Integer Literal: " + value; break;
			case TokenID::LIT_FLOAT: result += "Float Literal: " + value; break;
			case TokenID::END_OF_FILE: result += "EOF"; break;
			case TokenID::INVALID: result += "Invalid: " + value; break;
			default: result += "Unknown: " + value; break;
		}

		return result;
	}

	int Lexer::Peek() { return stream.peek(); }

	int Lexer::Read()
	{
		int result = stream.get();

		if (result == '\n')
		{
			position.line++;
			position.column = 1;
		}
		else if (result == '\t') { position.column += tabsize; }
		else { position.column++; }

		return result;
	}

	std::vector<Token> Tokenize(const std::string& _src, size_t _tabsize)
	{
		Lexer lexer(_src, _tabsize);
		std::vector<Token> result;

		while (true)
		{
			int peeked = lexer.Peek();

			while (std::isspace(peeked))
			{
				lexer.Read();
				peeked = lexer.Peek();
			}

			switch (peeked)
			{
				case ':': { result.push_back(Token(TokenID::SYM_COLON, lexer.GetPosition())); lexer.Read(); } break;
				case ';': { result.push_back(Token(TokenID::SYM_SEMICOLON, lexer.GetPosition())); lexer.Read(); } break;
				case '=': { result.push_back(Token(TokenID::SYM_EQUALS, lexer.GetPosition())); lexer.Read(); } break;
				case '+': { result.push_back(Token(TokenID::SYM_PLUS, lexer.GetPosition())); lexer.Read(); } break;
				case '-': { result.push_back(Token(TokenID::SYM_MINUS, lexer.GetPosition())); lexer.Read(); } break;
				case '*': { result.push_back(Token(TokenID::SYM_ASTERISK, lexer.GetPosition())); lexer.Read(); } break;
				case '/': { result.push_back(Token(TokenID::SYM_FSLASH, lexer.GetPosition())); lexer.Read(); } break;
				case '%': { result.push_back(Token(TokenID::SYM_PERCENT, lexer.GetPosition())); lexer.Read(); } break;
				case '(': { result.push_back(Token(TokenID::SYM_LPAREN, lexer.GetPosition())); lexer.Read(); } break;
				case ')': { result.push_back(Token(TokenID::SYM_RPAREN, lexer.GetPosition())); lexer.Read(); } break;
				default:
				{
					if (std::isalpha(peeked) || peeked == '_') // Identifier or Keyword
					{
						std::string value;
						Position position = lexer.GetPosition();

						do
						{
							value += lexer.Read();
							peeked = lexer.Peek();
						} while (std::isalnum(peeked) || peeked == '_');

						static std::unordered_map<std::string, TokenID> KEYWORDS = {
							{"let", TokenID::KW_LET }, {"int", TokenID::KW_INT }, {"float", TokenID::KW_FLOAT },
							{"bool", TokenID::KW_BOOL }, {"true", TokenID::KW_TRUE }, {"flase", TokenID::KW_FALSE }
						};

						auto keywordSearch = KEYWORDS.find(value);
						if (keywordSearch != KEYWORDS.end()) { result.push_back(Token(keywordSearch->second, position)); }
						else { result.push_back(Token(TokenID::IDENTIFIER, position, value)); }
					}
					else if (std::isdigit(peeked)) // Numeric Literal
					{
						std::string value;
						Position position = lexer.GetPosition();
						bool isFloat = false;

						do
						{
							value += lexer.Read();
							peeked = lexer.Peek();

							if (peeked == '.')
							{
								value += lexer.Read();

								if (isFloat) { break; }
								else
								{
									peeked = lexer.Peek();
									isFloat = true;
								}
							}

						} while (std::isdigit(peeked));

						if (value.back() == '.') // Make sure it doesn't end with a dot
						{
							PushDiagnostic(DiagnosticType::ERROR_InvalidFloatLit, position, value);
							result.push_back(Token(TokenID::INVALID, position, value));
						}
						else if (isFloat)
						{
							try
							{
								std::stod(value); //Attempt Conversion
								result.push_back(Token(TokenID::LIT_FLOAT, position, value));
							}
							catch (...)
							{
								PushDiagnostic(DiagnosticType::ERROR_FloatLitOutOfRange, position, value);
								result.push_back(Token(TokenID::INVALID, position, value));
							}
						}
						else
						{
							try
							{
								std::stoll(value); //Attempt Conversion
								result.push_back(Token(TokenID::LIT_INT, position, value));
							}
							catch (...)
							{
								PushDiagnostic(DiagnosticType::ERROR_IntLitOutOfRange, position, value);
								result.push_back(Token(TokenID::INVALID, position, value));
							}
						}
					}
					else if (peeked == EOF) // End of File
					{
						lexer.Read();
						result.push_back(Token(TokenID::END_OF_FILE, lexer.GetPosition()));
						return result;
					}
					else // Invalid
					{
						result.push_back(Token(TokenID::INVALID, lexer.GetPosition()));
						lexer.Read();
					}
				} break;
			}
		}
	}
#pragma endregion

	class TokenStream
	{
		std::vector<Token> tokens;
		size_t position;
	public:
		TokenStream(const std::vector<Token> _tokens) : tokens(_tokens), position(0) { }

		Token& Peek() { return tokens[position]; }
		Token& Read() { return tokens[position == tokens.size() ? position : position++]; }
		void Unread() { position = position == 0 ? 0 : (position - 1); }

		size_t GetPosition() { return position; }

		void Print()
		{
			for (auto tok : tokens)
				std::cout << tok.ToString() << std::endl;
		}
	};

	Expression* ParseExpression(TokenStream& _stream);

	Expression* ParseAtom(TokenStream& _stream)
	{
		Token& token = _stream.Read();
		Position start = token.position;

		switch (token.id)
		{
			case TokenID::KW_TRUE: return new Literal(true, start);
			case TokenID::KW_FALSE: return new Literal(false, start);
			case TokenID::LIT_INT: return new Literal(std::stoll(token.value), start);
			case TokenID::LIT_FLOAT: return new Literal(std::stod(token.value), start);
			case TokenID::SYM_LPAREN:
			{
				//Try get unit
				if (_stream.Peek().id == TokenID::SYM_RPAREN)
				{
					_stream.Read();
					return new Literal(UNIT(), start);
				}

				//Try get parenthesized expression
				Expression* expr = ParseExpression(_stream);

				if (expr)
				{
					Token& token = _stream.Read();

					if (token.id == TokenID::SYM_RPAREN)
						return expr;

					PushDiagnostic(DiagnosticType::ERROR_ExpectedClosingParen, token.position, token.value);
					_stream.Unread();
					return expr;
				}
			} break;
		}

		PushDiagnostic(DiagnosticType::ERROR_ExpectedAtom, token.position, token.value);
		_stream.Unread();
		return nullptr;
	}

	Expression* ParseExpression(TokenStream& _stream)
	{
		return ParseAtom(_stream);
	}

	Statement* ParseStatement(TokenStream& _stream)
	{
		Token& peeked = _stream.Peek();
		Expression* expr = ParseExpression(_stream);

		if (expr)
		{
			Token& token = _stream.Read();

			if (token.id == TokenID::SYM_SEMICOLON) { return expr; }
			else
			{
				PushDiagnostic(DiagnosticType::ERROR_ExpectedSemicolon, token.position, token.value);
				_stream.Unread();
				return expr;
			}
		}

		PushDiagnostic(DiagnosticType::ERROR_ExpectedStmt, peeked.position, peeked.value);
	}

	Node* Parse(const std::string& _src, size_t _tabsize)
	{
		TokenStream stream(Tokenize(_src, _tabsize));

		return ParseStatement(stream);
	}
}
