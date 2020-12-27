#pragma once
#include <string>

struct Lexeme
{
	std::string lexeme;
	std::string lexeme_type;
	int line;
	int symbol_number;
	
	Lexeme() = default;
	
	Lexeme(std::string lex, std::string type)
		: lexeme(lex), lexeme_type(type), line(), symbol_number() {}

	Lexeme(std::string&& lex, std::string&& type)
		: lexeme(lex), lexeme_type(type), line(), symbol_number() {}
};