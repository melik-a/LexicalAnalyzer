#include "Scanner.h"


LexicalScanner::LexicalScanner(std::string& file)
{
	_file.open(file);
	if (!_file)
	{
		std::cout << "\nsomething went wrong. invalid file name or path" << std::endl;
		throw "error -> invalid file name or path";
	}
	_current_state = States::START;
}


LexicalScanner::~LexicalScanner()
{
	if (_file)
		_file.close();
}


const std::vector<SyntaxToken>* LexicalScanner::scan()
{
	_lexeme_table.clear();
	std::string lexeme = "";

	char curr_symbol = ' ';

	while (!_file.eof())
	{
		curr_symbol = _file.get();
		switch (_current_state)
		{
			case States::START:
				start_state_changing(curr_symbol, lexeme);
				break;
			case States::ID:
				id_state_changing(curr_symbol, lexeme);
				break;
			case States::ARITHM_OPERATOR:
				arithm_operator_state_changing(curr_symbol, lexeme);
				break;
			case States::FLOAT_NUMBER:
				float_num_state_changing(curr_symbol, lexeme);
				break;
			case States::ASSIGNMENT:
				assignment_state_changing(curr_symbol, lexeme);
				break;
			case States::SEPARATOR:
				separator_state_changing(curr_symbol, lexeme);
				break;
			case States::COMMENT:
				comment_state_changing(curr_symbol, lexeme);
				break;
			case States::ERROR:
				std::cout << "ERROR state with error -> " << lexeme << std::endl;
				if (_file.is_open())
					_file.close();
				throw "error in the source file";
				break;
		}
	}
	
	if (_file.is_open())
		_file.close();
	_lexeme_table.push_back(SyntaxToken{ "", SyntaxTag::END_OF_FILE });
	return &_lexeme_table;
}


void LexicalScanner::start_state_changing(char symbol, std::string& lexeme)
{
	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexeme += symbol;
		_current_state = States::ARITHM_OPERATOR;
	}
	else if (symbol == ':')
	{
		lexeme += symbol;
		_current_state = States::ASSIGNMENT;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
	}
	else if (symbol == '\n' || symbol == '\t' || symbol == ' ')
	{
		return;
	}
	else
	{
		lexeme = "lexical scanner can't move from the starting point. maybe something wrong with source file";
		_current_state = States::ERROR;
	}
}


void LexicalScanner::id_state_changing(char symbol, std::string& lexeme) 
{
	if (is_digit(symbol) || is_letter(symbol))
	{
		lexeme += symbol;
	}
	else if (symbol == ':')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN });
		lexeme = symbol;
		_current_state = States::ASSIGNMENT;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN });
		lexeme = symbol;
		_current_state = States::ARITHM_OPERATOR;
	}
	else if (symbol == '{')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN });
		lexeme = "";
		_current_state = States::COMMENT;
	}
	else if (symbol == ';' || symbol == ')')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN });
		lexeme = symbol;
		_current_state = States::SEPARATOR;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN });
		lexeme = "";
		_current_state = States::START;
	}
	else
	{
		lexeme = "error on ID state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
	}
}


void LexicalScanner::assignment_state_changing(char symbol, std::string& lexeme)
{
	if (symbol == '=')
	{
		lexeme += symbol;
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::ASSIGN_TOKEN });
		lexeme = "";
		_current_state = States::START;
	}
	else
	{
		lexeme = "error on ASSIGNMENT state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
	}
}


void LexicalScanner::float_num_state_changing(char symbol, std::string& lexeme)
{
	if (is_digit(symbol) || symbol == '.' || symbol == 'e' || symbol == '+' || symbol == '-')
	{
		lexeme += symbol;
	}
	else if (symbol == '*' || symbol == '/')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER });
		lexeme = symbol;
		_current_state = States::ARITHM_OPERATOR;
	}
	else if (symbol == '{')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER });
		lexeme = "";
		_current_state = States::COMMENT;
	}
	else if (symbol == ';')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER });
		lexeme = symbol;
		_current_state = States::SEPARATOR;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER });
		lexeme = "";
		_current_state = States::START;
	}
	else
	{
		lexeme = "error on FLOAT_NUMBER state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
	}
}


void LexicalScanner::separator_state_changing(char symbol, std::string& lexeme)
{
	if (lexeme == "(")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::LP_TOKEN });
	else if (lexeme == ")")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::RP_TOKEN });
	else if (lexeme == ";")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::SEMICOLON_TOKEN });
	lexeme = "";
	
	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexeme += symbol;
		_current_state = States::ARITHM_OPERATOR;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		lexeme = "";
		_current_state = States::START;
	}
	else
	{
		lexeme = "error on SEPARATOR state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
	}
}


void LexicalScanner::arithm_operator_state_changing(char symbol, std::string& lexeme)
{
	if (lexeme == "+")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::PLUS_TOKEN });
	else if (lexeme == "-")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::MINUS_TOKEN });
	else if (lexeme == "*")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::STAR_TOKEN });
	else if (lexeme == "/")
		_lexeme_table.push_back(SyntaxToken{ lexeme, SyntaxTag::SLASH_TOKEN });
	
	lexeme = "";
	
	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
	}
	else if (symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		lexeme = "";
		_current_state = States::START;
	}
	else
	{
		lexeme = "error on ARITHM_OPERATOR state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
	}
}


void LexicalScanner::comment_state_changing(char symbol, std::string& lexeme)
{
	if (symbol == '}')
	{
		lexeme = "";
		_current_state = States::START;
	}
	else if (symbol == '\n')
	{
		lexeme = "not closed comment lexem";
		_current_state = States::ERROR;
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

