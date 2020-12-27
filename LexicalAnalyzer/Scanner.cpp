#include "Scanner.h"


LexicalScanner::LexicalScanner(std::string& file)
{
	_file.open(file);
	if (!_file)
	{
		std::cout << "\nsomething went wrong. invalid file name or path" << std::endl;
		throw "error -> invalid file name or path";
	}
	_current_state = START;
}


LexicalScanner::LexicalScanner(std::string&& file)
{
	_file.open(file);
	if (!_file)
	{
		std::cout << "\nsomething went wrong. invalid file name or path" << std::endl;
		throw "error -> invalid file name or path";
	}
	_current_state = START;
}


LexicalScanner::~LexicalScanner()
{
	if (_file)
		_file.close();
}


const std::vector<Lexeme>* LexicalScanner::scan()
{
	_lexem_table.clear();
	std::string lexem_attribute = "";

	char curr_symbol = ' ';

	while (!_file.eof())
	{
		curr_symbol = _file.get();
		switch (_current_state)
		{
			case START:
				start_state_changing(curr_symbol, lexem_attribute);
				break;
			case ID:
				id_state_changing(curr_symbol, lexem_attribute);
				break;
			case ARITHM_OPERATOR:
				arithm_operator_state_changing(curr_symbol, lexem_attribute);
				break;
			case FLOAT_NUMBER:
				float_num_state_changing(curr_symbol, lexem_attribute);
				break;
			case ASSIGNMENT:
				assignment_state_changing(curr_symbol, lexem_attribute);
				break;
			case SEPARATOR:
				separator_state_changing(curr_symbol, lexem_attribute);
				break;
			case COMMENT:
				comment_state_changing(curr_symbol, lexem_attribute);
				break;
			case ERROR:
				std::cout << "ERROR state with error-> " << lexem_attribute << std::endl;
				if (_file.is_open())
					_file.close();
				throw "error in the source file";
				break;
		}
	}
	if (_file.is_open())
		_file.close();
	return &_lexem_table;
}


void LexicalScanner::start_state_changing(char symbol, std::string& lexem_attribute)
{
	if (is_letter(symbol))
	{
		lexem_attribute += symbol;
		_current_state = ID;
	}
	else if (is_digit(symbol))
	{
		lexem_attribute += symbol;
		_current_state = FLOAT_NUMBER;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexem_attribute += symbol;
		_current_state = SEPARATOR;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexem_attribute += symbol;
		_current_state = ARITHM_OPERATOR;
	}
	else if (symbol == ':')
	{
		lexem_attribute += symbol;
		_current_state = ASSIGNMENT;
	}
	else if (symbol == '{')
	{
		_current_state = COMMENT;
	}
	else if (symbol == '\n' || symbol == '\t' || symbol == ' ')
	{
		return;
	}
	else
	{
		lexem_attribute = "lexical scanner can't move from the starting point. maybe something wrong with source file";
		_current_state = ERROR;
	}
}


void LexicalScanner::id_state_changing(char symbol, std::string& lexem_attribute) 
{
	if (is_digit(symbol) || is_letter(symbol))
	{
		lexem_attribute += symbol;
	}
	else if (symbol == ':')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ID") });
		lexem_attribute = symbol;
		_current_state = ASSIGNMENT;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ID") });
		lexem_attribute = symbol;
		_current_state = ARITHM_OPERATOR;
	}
	else if (symbol == '{')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ID") });
		lexem_attribute = "";
		_current_state = COMMENT;
	}
	else if (symbol == ';' || symbol == ')')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ID") });
		lexem_attribute = symbol;
		_current_state = SEPARATOR;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ID") });
		lexem_attribute = "";
		_current_state = START;
	}
	else
	{
		lexem_attribute = "error on ID state change. invalid character -> " + symbol;
		_current_state = ERROR;
	}
}


void LexicalScanner::assignment_state_changing(char symbol, std::string& lexem_attribute)
{
	if (symbol == '=')
	{
		lexem_attribute += symbol;
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ASSIGNMENT") });
		lexem_attribute = "";
		_current_state = START;
	}
	else
	{
		lexem_attribute = "error on ASSIGNMENT state change. invalid character -> " + symbol;
		_current_state = ERROR;
	}
}


void LexicalScanner::float_num_state_changing(char symbol, std::string& lexem_attribute)
{
	if (is_digit(symbol) || symbol == '.' || symbol == 'e' || symbol == '+' || symbol == '-')
	{
		lexem_attribute += symbol;
	}
	else if (symbol == '*' || symbol == '/')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("FLOAT_NUMBER") });
		lexem_attribute = symbol;
		_current_state = ARITHM_OPERATOR;
	}
	else if (symbol == '{')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("FLOAT_NUMBER") });
		lexem_attribute = "";
		_current_state = COMMENT;
	}
	else if (symbol == ';')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("FLOAT_NUMBER") });
		lexem_attribute = symbol;
		_current_state = SEPARATOR;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("FLOAT_NUMBER") });
		lexem_attribute = "";
		_current_state = START;
	}
	else
	{
		lexem_attribute = "error on FLOAT_NUMBER state change. invalid character -> " + symbol;
		_current_state = ERROR;
	}
}


void LexicalScanner::separator_state_changing(char symbol, std::string& lexem_attribute)
{
	_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("SEPARATOR") });
	lexem_attribute = "";
	if (is_letter(symbol))
	{
		lexem_attribute += symbol;
		_current_state = ID;
	}
	else if (is_digit(symbol))
	{
		lexem_attribute += symbol;
		_current_state = FLOAT_NUMBER;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexem_attribute += symbol;
		_current_state = SEPARATOR;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexem_attribute += symbol;
		_current_state = ARITHM_OPERATOR;
	}
	else if (symbol == '{')
	{
		_current_state = COMMENT;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		lexem_attribute = "";
		_current_state = START;
	}
	else
	{
		lexem_attribute = "error on SEPARATOR state change. invalid character -> " + symbol;
		_current_state = ERROR;
	}
}


void LexicalScanner::arithm_operator_state_changing(char symbol, std::string& lexem_attribute)
{
	_lexem_table.push_back(Lexeme{ lexem_attribute, std::string("ARITHM_OPERATOR") });
	lexem_attribute = "";
	if (is_letter(symbol))
	{
		lexem_attribute += symbol;
		_current_state = ID;
	}
	else if (is_digit(symbol))
	{
		lexem_attribute += symbol;
		_current_state = FLOAT_NUMBER;
	}
	else if (symbol == '(' || symbol == ')')
	{
		lexem_attribute += symbol;
		_current_state = SEPARATOR;
	}
	else if (symbol == '{')
	{
		_current_state = COMMENT;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		lexem_attribute = "";
		_current_state = START;
	}
	else
	{
		lexem_attribute = "error on ARITHM_OPERATOR state change. invalid character -> " + symbol;
		_current_state = ERROR;
	}
}


void LexicalScanner::comment_state_changing(char symbol, std::string& lexem_attribute)
{
	if (symbol == '}')
	{
		lexem_attribute = "";
		_current_state = START;
	}
	else if (symbol == '\n')
	{
		lexem_attribute = "not closed comment lexem";
		_current_state = ERROR;
	}
}


bool LexicalScanner::is_digit(char symbol)
{
	if (symbol >= '0' && symbol <= '9')
		return true;
	return false;
}


bool LexicalScanner::is_letter(char symbol)
{
	if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z') || symbol == '_')
		return true;
	return false;
}

