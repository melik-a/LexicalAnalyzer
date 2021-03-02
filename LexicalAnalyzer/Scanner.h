#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "States.h"
#include "SyntaxToken.h"
#include "SyntaxTag.h"

struct LexicalScanner
{
	LexicalScanner() = delete;
	LexicalScanner(std::string& file);
	~LexicalScanner();

	const std::vector<SyntaxToken>* scan();

	private:
		std::ifstream _file;
		States _current_state;
		std::vector<SyntaxToken> _lexeme_table;

		void start_state_changing(char symbol, std::string& lexeme);
		void id_state_changing(char symbol, std::string& lexeme);
		void assignment_state_changing(char symbol, std::string& lexeme);
		void float_num_state_changing(char symbol, std::string& lexeme);
		void separator_state_changing(char symbol, std::string& lexeme);
		void arithm_operator_state_changing(char symbol, std::string& lexeme);
		void comment_state_changing(char symbol, std::string& lexeme);

		bool is_letter(char symbol);
		bool is_digit(char symbol);
};

