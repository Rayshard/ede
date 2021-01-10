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
		bool IsEOF() { return tokens[position].id == TokenID::END_OF_FILE; }

		size_t GetPosition() { return position; }

		void Print()
		{
			for (auto tok : tokens)
				std::cout << tok.ToString() << std::endl;
		}
	};

	std::string ParseTypeName(TokenStream& _stream)
	{
		Token& token = _stream.Read();
		Position start = token.position;

		switch (token.id)
		{
			case TokenID::IDENTIFIER: return token.value;
			case TokenID::KW_INT: return "int";
			case TokenID::KW_FLOAT: return "float";
		}

		PushDiagnostic(DiagnosticType::ERROR_ExpectedTypeName, token.position, token.value);
		_stream.Unread();
		return "";
	}

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

	Expression* ParseBinopExpression(TokenStream& _stream, Expression* _lhs, size_t _minPrec)
	{
		struct BinopInfo { BinopOP op;  size_t precedence; bool leftAssoc; };

		//Map between binary operator symbols and if they are left associative
		static std::unordered_map<TokenID, BinopInfo> BINOPS = {
			{ TokenID::SYM_PLUS, { BinopOP::ADD, 0, true } }, { TokenID::SYM_MINUS, { BinopOP::SUB, 0, true } },
			{ TokenID::SYM_ASTERISK, { BinopOP::MUL, 1, true } }, { TokenID::SYM_FSLASH, { BinopOP::DIV, 1, true }}, { TokenID::SYM_PERCENT, { BinopOP::MOD, 1, true } }
		};

		Expression* result = _lhs;
		auto opSearch = BINOPS.find(_stream.Peek().id);

		while (opSearch != BINOPS.end())
		{
			BinopInfo initOp = opSearch->second;
			if (initOp.precedence < _minPrec) { break; }
			else { _stream.Read(); }

			Expression* rhs = ParseAtom(_stream);
			opSearch = BINOPS.find(_stream.Peek().id);

			while (opSearch != BINOPS.end())
			{
				BinopInfo postOp = opSearch->second;
				if (postOp.precedence <= initOp.precedence && (postOp.leftAssoc || postOp.precedence != initOp.precedence))
					break;

				rhs = ParseBinopExpression(_stream, rhs, postOp.precedence);
				opSearch = BINOPS.find(_stream.Peek().id);
			}

			result = new Binop(result, initOp.op, rhs, result->GetPosition());
		}

		return result;
	}

	Expression* ParseExpression(TokenStream& _stream)
	{
		Expression* atom = ParseAtom(_stream);
		return atom ? ParseBinopExpression(_stream, atom, 0) : atom;
	}

	VarDecl* ParseVarDecl(TokenStream& _stream)
	{
		Token& peeked = _stream.Peek();
		if (peeked.id != TokenID::KW_LET) { return nullptr; }
		else { _stream.Read(); }

		Position start = peeked.position;
		std::string varName;
		peeked = _stream.Peek();
		if (peeked.id != TokenID::IDENTIFIER)
		{
			PushDiagnostic(DiagnosticType::ERROR_ExpectedIdentifier, peeked.position, peeked.value);
			return nullptr;
		}
		else { varName = _stream.Read().value; }

		peeked = _stream.Peek();
		if (peeked.id != TokenID::SYM_COLON)
		{
			PushDiagnostic(DiagnosticType::ERROR_ExpectedColon, peeked.position, peeked.value);
			return nullptr;
		}
		else { _stream.Read(); }

		std::string typeName = ParseTypeName(_stream);
		if (typeName.empty()) { return nullptr; }

		peeked = _stream.Peek();
		if (peeked.id != TokenID::SYM_EQUALS)
		{
			PushDiagnostic(DiagnosticType::ERROR_ExpectedEquals, peeked.position, peeked.value);
			return nullptr;
		}
		else { _stream.Read(); }

		Expression* expr = ParseExpression(_stream);
		if (!expr)
		{
			PushDiagnostic(DiagnosticType::ERROR_ExpectedExpr, _stream.Peek().position, _stream.Peek().value);
			return nullptr;
		}
		else { return new VarDecl(varName, typeName, expr, start); }
	}

	Statement* ParseStatement(TokenStream& _stream)
	{
		Statement* result = nullptr;
		Token& start = _stream.Peek();

		if ((result = ParseVarDecl(_stream)) || (result = ParseExpression(_stream)))
		{
			Token& token = _stream.Read();
			if (token.id != TokenID::SYM_SEMICOLON)
			{
				PushDiagnostic(DiagnosticType::ERROR_ExpectedSemicolon, token.position, token.value);
				_stream.Unread();
			}
		}
		else { PushDiagnostic(DiagnosticType::ERROR_ExpectedStmt, start.position, start.value); }

		return result;
	}

	Node* Parse(const std::string& _src, size_t _tabsize)
	{
		TokenStream stream(Tokenize(_src, _tabsize));
		std::vector<Statement*> statements;

		while (!stream.IsEOF())
			statements.push_back(ParseStatement(stream));

		return new Block(statements, (!statements.empty() && statements.front()) ? statements.front()->GetPosition() : Position(1, 1));
	}
}
