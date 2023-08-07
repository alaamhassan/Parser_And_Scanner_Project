#include "SCANNER.h"
#include "Parser.h"
#include "TOKEN.h"
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/*********************************HELPER_FuncitonS*************************************/

/*********************************HELPER_FuncitonS*************************************/

//intialize the scanner by:
//assigning location in memory
//initialize it's contents
Scanner_T* init_scanner(std::string content)
{
	/* assign location in memory for the scanner*/
	Scanner_T* scanner = new Scanner_T();

	scanner->content = content;
	scanner->current_index = 0;
	scanner->current_char = content[scanner->current_index];

	return scanner;

}


//advance to the next char if there still charchters left
void scanner_advance_to_next_char(Scanner_T* scanner) {


	/*check if current char is not the end of the scanner->content array*/
	/* or : if current_index is smaller than the length of the scanner->content*/

	if (scanner->current_char != '\0' && scanner->current_index < (scanner->content).length())
	{
		scanner->current_index++;
		scanner->current_char = scanner->content[scanner->current_index];
	}
}

//ignore next char if it's a wihteSpace or a newLine
void scanner_skip_whitSpaces_and_newLines(Scanner_T* scanner) {

	while (scanner->current_char == ' ' || scanner->current_char == '\n' || scanner->current_char == '\r' || scanner->current_char == '\t') ////////////////
	{
		scanner->current_index++;
		scanner->current_char = scanner->content[scanner->current_index];
	}
}

//ignore comments
void scanner_skip_comments(Scanner_T* scanner) {

	/*advanct to next char after { */
	scanner_advance_to_next_char(scanner);

	while (scanner->current_char != '}')
		scanner_advance_to_next_char(scanner);

	/*advance to next char after } */
	scanner_advance_to_next_char(scanner);
}

//return curruentChar of the string 
char scanner_get_currentChar(Scanner_T* scanner) {

	return scanner->current_char;

}

string scanner_get_currentChar_as_string(Scanner_T* scanner) {
	string s = "";
	s += scanner->current_char;
	return s;
}

//get the whole identifier and initilaize the token with the identifier token_name
Token_T* scanner_get_identifier(Scanner_T* scanner) {

	string identifier = "";

	//take the identifier while it's an alphabet 
	while (isalpha(scanner->current_char))
	{
		char current_char_in_identifier = scanner_get_currentChar(scanner);

		/*append the current_char to the end of the identifier*/
		identifier += current_char_in_identifier;

		scanner_advance_to_next_char(scanner);
	}

	/*name the current token*/
	TOKEN_NAMES token_name;
	token_name = IDENTIFIER_TOKEN;

	return init_token(token_name, identifier);
}

//get the whole number and initilaize the token with the number token_name
Token_T* scanner_get_number(Scanner_T* scanner) {
	string number = "";
	while (isdigit(scanner->current_char)) {
		number += scanner_get_currentChar_as_string(scanner);
		scanner_advance_to_next_char(scanner);
	}
	return init_token(NUMBER_TOKEN, number);
}

Token_T* scanner_advance_with_Token(Scanner_T* scanner, Token_T* token) {

	scanner_advance_to_next_char(scanner);

	return token;
}


//string TokenNames_To_String(TOKEN_NAMES token_name) {
//	switch (token_name)
//	{
//	case TOKEN_NAMES::SEMICOLON_TOKEN: return "SEMICOLON";
//	case TOKEN_NAMES::IF_TOEKN: return "IF";
//	case TOKEN_NAMES::THEN_TOKEN: return "THEN";
//	case TOKEN_NAMES::END_TOKEN: return "END";
//	case TOKEN_NAMES::REPEAT_TOKEN: return "REPEAT";
//	case TOKEN_NAMES::UNTIL_TOKEN: return "UNTIL";
//	case TOKEN_NAMES::IDENTIFIER_TOKEN: return "IDENTIFIER";
//	case TOKEN_NAMES::ASSIGN_TOKEN: return "ASSIGN";
//	case TOKEN_NAMES::READ_TOKEN: return "READ";
//	case TOKEN_NAMES::WRITE_TOKEN: return "WRITE";
//	case TOKEN_NAMES::LESSTHAN_TOKEN: return "LESSTHAN";
//	case TOKEN_NAMES::EQUAL_TOKEN: return "EQUAL";
//	case TOKEN_NAMES::PLUS_TOKEN: return "PLUS";
//	case TOKEN_NAMES::MINUS_TOKEN: return "MINUS";
//	case TOKEN_NAMES::MULT_TOKEN: return "MULT";
//	case TOKEN_NAMES::DIV_TOKEN: return "DIV";
//	case TOKEN_NAMES::OPENBRACKET_TOKEN: return "OPENBRACKET";
//	case TOKEN_NAMES::CLOSEBRACKET_TOKEN: return "CLOSEBRACKET";
//	case TOKEN_NAMES::NUMBER_TOKEN: return "NUMBER";
//	case TOKEN_NAMES::INVALID_TOKEN: return "INVALID";
//
//	default: throw std::invalid_argument("Unimplemented enum item");
//	}
//}

Token_T* scanner_get_identifier_type(Scanner_T* scanner, Token_T* token) {
	if (token->token_value == "if") {
		token->token_name = IF_TOEKN;
		return token;
	}
	if (token->token_value == "then") {
		token->token_name = THEN_TOKEN;
		return token;
	}
	if (token->token_value == "end") {
		token->token_name = END_TOKEN;
		return token;
	}
	if (token->token_value == "repeat") {
		token->token_name = REPEAT_TOKEN;
		return token;
	}
	if (token->token_value == "until") {
		token->token_name = UNTIL_TOKEN;
		return token;
	}
	if (token->token_value == "read") {
		token->token_name = READ_TOKEN;
		return token;
	}
	if (token->token_value == "write") {
		token->token_name = WRITE_TOKEN;
		return token;
	}
	if (token->token_value == "else") {
		token->token_name = ELSE_TOKEN;
		return token;
	}
	return init_token(IDENTIFIER_TOKEN, token->token_value);
}

Token_T* scanner_check_assign(Scanner_T* scanner) {
	if (scanner_get_currentChar(scanner) == ':') {
		scanner_advance_to_next_char(scanner);
		if (scanner_get_currentChar(scanner) == '=') {
			return init_token(ASSIGN_TOKEN, ":=");
		}
		return init_token(INVALID_TOKEN, ":");
	}
}

Token_T* scanner_get_next_token(Scanner_T* scanner) {
	while (scanner->current_char != '\0' && scanner->current_index < scanner->content.length()) {
		if (scanner->current_char == ' ' || scanner->current_char == '\n') {
			scanner_skip_whitSpaces_and_newLines(scanner);
			continue;
		}
		if (scanner->current_char == '{') {
			scanner_skip_comments(scanner);
			continue;
		}
		if (isdigit(scanner->current_char))
			return scanner_get_number(scanner);
		if (isalpha(scanner->current_char)) {
			Token_T* token = scanner_get_identifier(scanner);
			return scanner_get_identifier_type(scanner, token);
		}

		switch (scanner->current_char) {
		case '<': return scanner_advance_with_Token(scanner, init_token(LESSTHAN_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case '=': return scanner_advance_with_Token(scanner, init_token(EQUAL_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case '+': return scanner_advance_with_Token(scanner, init_token(PLUS_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case '-': return scanner_advance_with_Token(scanner, init_token(MINUS_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case '*': return scanner_advance_with_Token(scanner, init_token(MULT_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case '/': return scanner_advance_with_Token(scanner, init_token(DIV_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case '(': return scanner_advance_with_Token(scanner, init_token(OPENBRACKET_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case ')': return scanner_advance_with_Token(scanner, init_token(CLOSEBRACKET_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case ';': return scanner_advance_with_Token(scanner, init_token(SEMICOLON_TOKEN, scanner_get_currentChar_as_string(scanner))); break;
		case ':': {
			Token_T* token = scanner_check_assign(scanner);
			if (token->token_name == ASSIGN_TOKEN)
				return scanner_advance_with_Token(scanner, token);
			else
				return token; break;
		}
		default: return scanner_advance_with_Token(scanner, init_token(INVALID_TOKEN, scanner_get_currentChar_as_string(scanner)));
		}
	}

	return NULL;
}

/*********************************Main_Funciton*************************************/


// function that read an input file containing Tiny language syntax 
// to generate a vector containing 
// // all tokens in the file
// vector<Token_T*> scanner_get_all_Tokens(string input_file_path)
// {
// 	/*declaring variables*/
// 	ifstream input_file;
// 	vector<Token_T*> TokenVector;

// 	/*reading input file name*/
// 	input_file.open(input_file_path, fstream::in);
// 	stringstream buffer;
// 	buffer << input_file.rdbuf();
// 	string input_as_string(buffer.str());
// 	input_file.close();

// 	/*scanning input string*/
// 	Scanner_T* lexer = init_scanner(input_file_path);
// 	Token_T* token = NULL;


// 	/*storing generated tokens in a TokenVector*/
// 	while ((token = scanner_get_next_token(lexer)) != NULL)
// 	{
// 		if (token->token_name == INVALID_TOKEN)
// 		{
// 			/*if this happend then we reached an error*/
// 			Error_message->error_message = "INVALID TOKEN => " + token->token_value;
// 			Error_message->is_error = 1;
// 			return TokenVector;
// 		}

// 		TokenVector.push_back(token);

// 	}

// 	return TokenVector;
// }



