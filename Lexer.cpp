#include "pch.h"
#include "Lexer.h"

namespace ede::lexer
{
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

	int Lexer::Peek(std::ifstream& _stream) { return _stream.peek(); }

	int Lexer::Read(std::ifstream& _stream)
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

	std::vector<Token> Lexer::Tokenize(std::ifstream& _stream)
	{
		std::vector<Token> result;

		while (true)
		{
			int peeked = Peek(_stream);

			while (std::isspace(peeked))
			{
				Read(_stream);
				peeked = Peek(_stream);
			}

			switch (peeked)
			{
				case ':': { result.push_back(Token(TokenID::SYM_COLON, GetPosition())); Read(_stream); } break;
				case ';': { result.push_back(Token(TokenID::SYM_SEMICOLON, GetPosition())); Read(_stream); } break;
				case '=': { result.push_back(Token(TokenID::SYM_EQUALS, GetPosition())); Read(_stream); } break;
				case '+': { result.push_back(Token(TokenID::SYM_PLUS, GetPosition())); Read(_stream); } break;
				case '-': { result.push_back(Token(TokenID::SYM_MINUS, GetPosition())); Read(_stream); } break;
				case '*': { result.push_back(Token(TokenID::SYM_ASTERISK, GetPosition())); Read(_stream); } break;
				case '/': { result.push_back(Token(TokenID::SYM_FSLASH, GetPosition())); Read(_stream); } break;
				case '%': { result.push_back(Token(TokenID::SYM_PERCENT, GetPosition())); Read(_stream); } break;
				case '(': { result.push_back(Token(TokenID::SYM_LPAREN, GetPosition())); Read(_stream); } break;
				case ')': { result.push_back(Token(TokenID::SYM_RPAREN, GetPosition())); Read(_stream); } break;
				default:
				{
					if (std::isalpha(peeked) || peeked == '_') // Identifier or Keyword
					{
						std::string value;
						Position position = GetPosition();

						do
						{
							value += Read(_stream);
							peeked = Peek(_stream);
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
						Position position = GetPosition();
						bool isFloat = false;

						do
						{
							value += Read(_stream);
							peeked = Peek(_stream);

							if (peeked == '.')
							{
								value += Read(_stream);

								if (isFloat) { break; }
								else
								{
									peeked = Peek(_stream);
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
						Read(_stream);
						result.push_back(Token(TokenID::END_OF_FILE, GetPosition()));
						return result;
					}
					else // Invalid
					{
						result.push_back(Token(TokenID::INVALID, GetPosition()));
						Read(_stream);
					}
				} break;
			}
		}
	}
};