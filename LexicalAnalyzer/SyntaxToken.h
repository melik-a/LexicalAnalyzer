#pragma once

#include <string>
#include "SyntaxTag.h"

struct SyntaxToken
{
	std::string lexeme;
	SyntaxTag token_type;
	
	int line;
	int symbol_pos;
	
	SyntaxToken() = default;
	
	SyntaxToken(std::string lex, SyntaxTag lex_type)
		: lexeme(lex), token_type(lex_type), line(), symbol_pos() {}

	SyntaxToken(std::string lex, SyntaxTag lex_type, int f_line, int f_pos)
		: lexeme(lex), token_type(lex_type), line(f_line), symbol_pos(f_pos) {}

	std::string token_type_to_string()
	{
		switch (token_type)
		{
			case SyntaxTag::END_OF_FILE:
				return "END_OF_FILE";
				break;
			case SyntaxTag::ID_TOKEN:
				return "ID_TOKEN";
				break;
			case SyntaxTag::ASSIGN_TOKEN:
				return "ASSIGN_TOKEN";
				break;
			case SyntaxTag::LP_TOKEN:
				return "LP_TOKEN";
				break;
			case SyntaxTag::RP_TOKEN:
				return "RP_TOKEN";
				break;
			case SyntaxTag::PLUS_TOKEN:
				return "PLUS_TOKEN";
				break;
			case SyntaxTag::MINUS_TOKEN:
				return "MINUS_TOKEN";
				break;
			case SyntaxTag::STAR_TOKEN:
				return "STAR_TOKEN";
				break;
			case SyntaxTag::SLASH_TOKEN:
				return "SLASH_TOKEN";
				break;
			case SyntaxTag::FLOAT_NUMBER:
				return "FLOAT_NUMBER";
				break;
			case SyntaxTag::SEMICOLON_TOKEN:
				return "SEMICOLON_TOKEN";
				break;
		}
		return "UNKNOWN_TOKEN";
	}

};