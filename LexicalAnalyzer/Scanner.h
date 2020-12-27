#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "States.h"
#include "Lexeme.h"

struct LexicalScanner
{
	LexicalScanner() = delete;
	LexicalScanner(std::string& file);
	LexicalScanner(std::string&& file);
	~LexicalScanner();

	const std::vector<Lexeme>* scan();

	private:
		std::ifstream _file;
		states _current_state;
		std::vector<Lexeme> _lexem_table;

		void start_state_changing(char symbol, std::string& lexem_attribute);
		void id_state_changing(char symbol, std::string& lexem_attribute);
		void assignment_state_changing(char symbol, std::string& lexem_attribute);
		void float_num_state_changing(char symbol, std::string& lexem_attribute);
		void separator_state_changing(char symbol, std::string& lexem_attribute);
		void arithm_operator_state_changing(char symbol, std::string& lexem_attribute);
		void comment_state_changing(char symbol, std::string& lexem_attribute);

		bool is_letter(char symbol);
		bool is_digit(char symbol);
};

