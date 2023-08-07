#ifndef PARSER_H
#define PARSER_H
#include "SCANNER.h"
#include "SYNTAXTREE.h"
#include "ERROR.h"
using namespace std;

extern int is_error;
//extern string error_msg;
extern string error_message; //////////

typedef struct PARSER_STRUCT
{
	int current_index_in_Tokens_Vector;
	Token_T* current_Token;
	Token_T* next_Token;
}Parser_T;


Parser_T* init_Parser(int current_index_in_Tokens_Vector);

void copy_childrens(SyntaxTree_T* tree1, SyntaxTree_T* tree2);

std::string TokenNames_To_String(TOKEN_NAMES token_name);

std::string print_token_types(std::vector<TOKEN_NAMES> token_names);

int advance_parser(Parser_T* parser, std::vector<TOKEN_NAMES> token_names, int is_not_last_token);

SyntaxTree_T* parse_program(Parser_T* parser);

SyntaxTree_T* parse_stmt_seq(Parser_T* parser);

SyntaxTree_T* parse_statement(Parser_T* parser);

SyntaxTree_T* parse_if_stmt(Parser_T* parser);

SyntaxTree_T* parse_repeat_stmt(Parser_T* parser);

SyntaxTree_T* parse_assign_stmt(Parser_T* parser);

SyntaxTree_T* parse_read_stmt(Parser_T* parser);

SyntaxTree_T* parse_write_stmt(Parser_T* parser);

SyntaxTree_T* parse_terminal(std::string terminal);

SyntaxTree_T* parse_exp(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_ComparisonOp(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_SimpleExp(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_AddOp(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_term(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_MulOp(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_factor(Parser_T* parser, SyntaxTree_T* syntaxTree);

SyntaxTree_T* parse_number(Parser_T* parser);

SyntaxTree_T* parse_identifier(Parser_T* parser);

std::vector<Token_T*> scanner_get_all_Tokens(std::string input_file_path);

std::vector<SyntaxTree_T*> generate_programm_tree(std::string input_file_path);
#endif
