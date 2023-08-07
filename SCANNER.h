#ifndef SCANNER_H
#define SCANNER_H
#include "TOKEN.h"
typedef struct Scanner_Struct
{
	std::string content;
	unsigned int current_index;
	char current_char;
} Scanner_T;
Scanner_T* init_scanner(std::string content);

void scanner_advance_to_next_char(Scanner_T* scanner);

void scanner_skip_whitSpaces_and_newLines(Scanner_T* scanner);

void scanner_skip_comments(Scanner_T* scanner);

Token_T* scanner_get_next_token(Scanner_T* scanner);

Token_T* scanner_get_identifier(Scanner_T* scanner);

Token_T* scanner_get_number(Scanner_T* scanner);

Token_T* scanner_advance_with_Token(Scanner_T* scanner, Token_T* token);

Token_T* scanner_get_identifier_type(Scanner_T* scanner, Token_T* token);

Token_T* scanner_check_assign(Scanner_T* scanner);

char scanner_get_currentChar(Scanner_T* scanner);
#endif





