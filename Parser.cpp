#include "Parser.h"
#include "ERROR.h"
#include "TOKEN.h"
#include "SCANNER.h"
#include <string.h>
#include <iostream>
#include<fstream>
#include<sstream>
#include <map>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <vector>

using namespace std;

std::vector<Token_T*> Tokens_Vector;
int current_line_number;
string error_message = "";
int is_error = 0;
//Parser_T* globalParser;
/*********************************HELPER_FuncitonS*************************************/

//intialize the parser by:
//assigning location in memory
//initialize it's contents
Parser_T* init_Parser(int current_index_in_Tokens_Vector)
{
	Parser_T* Parser = new Parser_T();

	Parser->current_index_in_Tokens_Vector = current_index_in_Tokens_Vector;
	Parser->current_Token = Tokens_Vector[current_index_in_Tokens_Vector];
	Parser->next_Token = Tokens_Vector[++current_index_in_Tokens_Vector];

	return Parser;
}


void copy_childrens(SyntaxTree_T* tree1, SyntaxTree_T* tree2)
{
	for (int i = 0; i < tree1->children.size(); i++)
	{
		tree2->children.push_back(tree1->children[i]);
	}



}
std::string TokenNames_To_String(TOKEN_NAMES token_name) {
	switch (token_name)
	{
	case TOKEN_NAMES::SEMICOLON_TOKEN: return "SEMICOLON";
	case TOKEN_NAMES::IF_TOEKN: return "IF";
	case TOKEN_NAMES::THEN_TOKEN: return "THEN";
	case TOKEN_NAMES::ELSE_TOKEN: return "ELSE";
	case TOKEN_NAMES::END_TOKEN: return "END";
	case TOKEN_NAMES::REPEAT_TOKEN: return "REPEAT";
	case TOKEN_NAMES::UNTIL_TOKEN: return "UNTIL";
	case TOKEN_NAMES::IDENTIFIER_TOKEN: return "IDENTIFIER";
	case TOKEN_NAMES::ASSIGN_TOKEN: return "ASSIGN";
	case TOKEN_NAMES::READ_TOKEN: return "READ";
	case TOKEN_NAMES::WRITE_TOKEN: return "WRITE";
	case TOKEN_NAMES::LESSTHAN_TOKEN: return "LESSTHAN";
	case TOKEN_NAMES::EQUAL_TOKEN: return "EQUAL";
	case TOKEN_NAMES::PLUS_TOKEN: return "PLUS";
	case TOKEN_NAMES::MINUS_TOKEN: return "MINUS";
	case TOKEN_NAMES::MULT_TOKEN: return "MULT";
	case TOKEN_NAMES::DIV_TOKEN: return "DIV";
	case TOKEN_NAMES::OPENBRACKET_TOKEN: return "OPENBRACKET";
	case TOKEN_NAMES::CLOSEBRACKET_TOKEN: return "CLOSEBRACKET";
	case TOKEN_NAMES::NUMBER_TOKEN: return "NUMBER";
	case TOKEN_NAMES::INVALID_TOKEN: return "INVALID";

	default: throw std::invalid_argument("Unimplemented enum item");
	}
}




std::string print_token_types(std::vector<TOKEN_NAMES> token_names)
{
	std::string token_types_string = "";
	for (int i = 0; i < token_names.size(); i++)
	{
		token_types_string += TokenNames_To_String(token_names[i]);

		if (i != token_names.size() - 1)
			token_types_string += ",";
	}
	return token_types_string;
}


//advance the parser by
//advance to the next token
int advance_parser(Parser_T* parser, std::vector<TOKEN_NAMES> token_names,int is_not_last_token)
{

	if (parser->next_Token != NULL)
	{
		for (int i = 0; i < token_names.size(); i++) {

			if (parser->next_Token->token_name == token_names[i])
			{

				if (parser->current_index_in_Tokens_Vector + 1 != Tokens_Vector.size() - 1)
				{
					/*advance the current index to get the next token*/
					parser->current_index_in_Tokens_Vector++;

					/*advance the to the next Tokens*/
					parser->current_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector];

					/*if (parser->current_index_in_Tokens_Vector + 1 == Tokens_Vector.size())
						parser->next_Token = NULL;*/
					parser->next_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector + 1];
				}
				else
				{
					/*advance the current index to get the next token*/
					parser->current_index_in_Tokens_Vector++;

					/*advance the to the next Tokens*/
					parser->current_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector];

					/*if (parser->current_index_in_Tokens_Vector + 1 == Tokens_Vector.size())
						parser->next_Token = NULL;*/
					parser->next_Token = NULL;
				}

				/*if this happend => then there is no error*/
                is_error = 0;
				return 1;
			}

		}

		/*if the code reached here=> then there is an error*/
		error_message = "\nError: " + parser->next_Token->token_value + " =>" + TokenNames_To_String(parser->next_Token->token_name) +
			"\nExpected input: " + print_token_types(token_names);
		is_error = 1;
		return 0;

	}
	else if (is_not_last_token)
	{


		/*if the code reached here=> then there is an error */
		error_message = TokenNames_To_String(parser->current_Token->token_name) + " can't be the last token!";
		is_error = 1;
		return 0;
	}
	else
		return 1;


}




SyntaxTree_T* parse_program(Parser_T* parser)
{
	return parse_stmt_seq(parser);
}

SyntaxTree_T* parse_stmt_seq(Parser_T* parser)
{
	SyntaxTree_T* tree = parse_statement(parser);
	SyntaxTree_T* temp = tree;


	while (parser->current_Token->token_name == SEMICOLON_TOKEN && parser->next_Token != NULL)
	{
		std::vector<TOKEN_NAMES> token_names = { IF_TOEKN ,REPEAT_TOKEN ,IDENTIFIER_TOKEN ,READ_TOKEN ,WRITE_TOKEN };
		int advance_no_error = advance_parser(parser, token_names,0);
		if (!advance_no_error) {
			return temp;
		}
		SyntaxTree_T* new_tree = parse_statement(parser);
		if (temp != NULL) {
			temp->neighbour = new_tree;
		}
		temp = new_tree;
		
	}
	// If the input code is terminated with a ; then return NULL
	if (parser->current_Token->token_name == SEMICOLON_TOKEN && parser->next_Token == NULL) {
		is_error = 1;
		error_message = "\nError: Unexpected \";\"";/*"Error in line: " + to_string(current_line_number) +*/
			
		return NULL;
	}
	return tree;
}

SyntaxTree_T* parse_statement(Parser_T* parser)
{
	int type = parser->current_Token->token_name;
	switch (type)
	{
	case IF_TOEKN: return parse_if_stmt(parser);
	case REPEAT_TOKEN: return parse_repeat_stmt(parser);
	case IDENTIFIER_TOKEN: return parse_assign_stmt(parser); //assign
	case READ_TOKEN: return parse_read_stmt(parser);
	case WRITE_TOKEN: return parse_write_stmt(parser);

	default: return NULL;
	}
}

SyntaxTree_T* parse_if_stmt(Parser_T* parser)
{
	SyntaxTree_T* if_tree = new SyntaxTree_T();

	//std::vector<TOKEN_NAMES> token_names ={IF_TOEKN};
	std::vector<TOKEN_NAMES> token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };
	if (advance_parser(parser, token_names,1) == 0)
		return NULL;

	if_tree->Node_name = "if";
	if_tree->isStatement = true;

	if_tree = parse_exp(parser, if_tree);
	//parse_exp(parser, if_tree);
	if (if_tree == NULL)
		return NULL;


	// advance (THEN) token
    if (parser->current_Token->token_name != THEN_TOKEN) {
        is_error = 1;
        error_message = "Error: " + parser->current_Token->token_value + "\nExpected THEN token";
        return NULL;
    }

    token_names = { IF_TOEKN ,REPEAT_TOKEN ,IDENTIFIER_TOKEN ,READ_TOKEN ,WRITE_TOKEN };
	if (advance_parser(parser, token_names,1) == 0)
		return NULL;



	SyntaxTree_T* stmt_seq = parse_stmt_seq(parser);
	if (stmt_seq == NULL)
		return NULL;


	//if_tree->children.push_back(exp);
	if_tree->children.push_back(stmt_seq);

	// optional part
	if (parser->current_Token->token_name == ELSE_TOKEN) {
		token_names = { IF_TOEKN ,REPEAT_TOKEN ,IDENTIFIER_TOKEN ,READ_TOKEN ,WRITE_TOKEN };
		if (advance_parser(parser, token_names,1) == 0)
			return NULL;

		SyntaxTree_T* stmt_seq2 = parse_stmt_seq(parser);
		if (stmt_seq2 == NULL)
			return NULL;


		if_tree->children.push_back(stmt_seq2);
	}

	// end part
	/*EXPECTED TOKEN AFTER END_TOKEN*/
    if (parser->current_Token->token_name != END_TOKEN) {
        is_error = 1;
        error_message = "Error: " + parser->current_Token->token_value + "\nExpected END token";
        return NULL;
    }

	/*if this is the last statement=> then the parsing ended
	check if the current_index_in_Tokens_Vector token +1 equal the vector size-1
	*/
	//if (parser->current_index_in_Tokens_Vector + 1 == Tokens_Vector.size() - 1)
	//{
	//	/*if we reached here it means the parsing ended*/
	//	parser->current_index_in_Tokens_Vector++;
	//	parser->current_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector];
	//	parser->next_Token = NULL;

	//}
	if (parser->next_Token != NULL)
	{
		/*if we reached here it means there is still statements to parse*/
		token_names = { SEMICOLON_TOKEN,END_TOKEN,ELSE_TOKEN,UNTIL_TOKEN };
		if (advance_parser(parser, token_names,0) == NULL)
			return NULL;
	}


	// if(parser->current_Token->token_name == END_TOKEN){
	// 	advance_parser(parser, END_TOKEN);
	// }
	// else 
	// {
	// 	cout << "from parse_if_stmt Expected (end) token" << endl;
	// }

	return if_tree;
}

SyntaxTree_T* parse_repeat_stmt(Parser_T* parser)
{
	SyntaxTree_T* repeat_tree = new SyntaxTree_T();

	repeat_tree->Node_name = parser->current_Token->token_value;
	repeat_tree->isStatement = true;

	/*EXPECTED TOKEN AFTER REPEAT_TOKEN*/
	std::vector<TOKEN_NAMES> token_names = { IF_TOEKN ,REPEAT_TOKEN ,IDENTIFIER_TOKEN ,READ_TOKEN ,WRITE_TOKEN };
	if (advance_parser(parser, token_names,1) == 0) {
		return NULL;
	}

	SyntaxTree_T* stmt_seq = parse_stmt_seq(parser);
	if (stmt_seq == NULL)
		return NULL;

	repeat_tree->children.push_back(stmt_seq);

	// until part
    if (parser->current_Token->token_name != UNTIL_TOKEN) {
        is_error = 1;
        error_message = "Error: " + parser->current_Token->token_value + "\nExpected Until token";
        return NULL;
    }

	token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };

	if (advance_parser(parser, token_names,1) == 0)
		return NULL;

	// if(parser->current_Token->token_name == UNTIL_TOKEN)
	// {
	// 	advance_parser(parser, UNTIL_TOKEN);
	// }
	// else 
	// {
	// 	cout << "from parse_repeat_stmt, Expected (untill)} " << endl;
	// }

	repeat_tree = parse_exp(parser, repeat_tree);
	//parse_exp(parser,repeat_tree);

	//repeat_tree->children.push_back(exp);

	return repeat_tree;
}

SyntaxTree_T* parse_assign_stmt(Parser_T* parser)
{
	SyntaxTree_T* assign_tree = new SyntaxTree_T();

	string id_name = parser->current_Token->token_value;

	/*EXPECTED TOKEN AFTER IDENTIFIER_TOKEN=> , MULT_TOKEN, DIV_TOKEN, PLUS_TOKEN, MINUS_TOKEN*/
    std::vector<TOKEN_NAMES> token_names = {ASSIGN_TOKEN}; /*make sure of this*********/
	if (advance_parser(parser, token_names,1) == NULL)
		return NULL;

	/*EXPECTED TOKEN AFTER ASSIGN_TOKEN*/
	token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };
	if (advance_parser(parser, token_names,1) == NULL)
		return NULL;


	assign_tree->Node_name = "assign\n(" + id_name + ")";
	assign_tree->isStatement = true;
	// assign_tree->children.push_back(parse_exp(parser,assign_tree));

	assign_tree = parse_exp(parser, assign_tree);

	return assign_tree;
}

SyntaxTree_T* parse_read_stmt(Parser_T* parser)
{
	// read-stmt --> read id

	SyntaxTree_T* read = new SyntaxTree_T();
	SyntaxTree_T* id = new SyntaxTree_T();


	/*EXPECTED TOKEN AFTER READ*/
	std::vector<TOKEN_NAMES> token_names = { IDENTIFIER_TOKEN };
	if (advance_parser(parser, token_names,1) == NULL)
		return NULL;


	string id_name = parser->current_Token->token_value;
	// construct the name of the node
	read->Node_name = "read\n(" + id_name + ")";
	read->isStatement = true;
	read->children = {};

	/*EXPECTED TOKEN AFTER READ IDENTIFIER*/
	token_names = { SEMICOLON_TOKEN ,END_TOKEN,ELSE_TOKEN,UNTIL_TOKEN };
	if (advance_parser(parser, token_names,0) == NULL)
		return NULL;

	return read;
}

SyntaxTree_T* parse_write_stmt(Parser_T* parser)
{
	// write-stmt --> write exp
	SyntaxTree_T* write = new SyntaxTree_T();
	SyntaxTree_T* exp = new SyntaxTree_T();

	// construct the name of the node
	write->Node_name = parser->current_Token->token_value;
	write->isStatement = true;

	/*EXPECTED TOKEN AFTER WRITE_TOKEN*/
	std::vector<TOKEN_NAMES> token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };
	if (advance_parser(parser, token_names,1) == NULL)
		return NULL;

	// construct exp node


	write = parse_exp(parser, write);
	//parse_exp(parser, write);

	return write;
}

bool is_statement(int next_Token) {
	if (next_Token == IF_TOEKN || next_Token == REPEAT_TOKEN || next_Token == ASSIGN_TOKEN ||
		next_Token == READ_TOKEN || next_Token == WRITE_TOKEN) {
		return true;
	}
	return false;
}


//function to parse terminal by 
// naming the node
// and initialize it's other attributes
SyntaxTree_T* parse_terminal(std::string terminal)
{
	///*initalize a syntaxTree*/
	//SyntaxTree_T* syntaxTree = new SyntaxTree_T();

	///*name the node => terminal_name*/
	//syntaxTree->Node_name =terminal;

	///*make childern vector empty => leaf node doesn't have any children*/
	//syntaxTree->children = {};

	///*leaf node dosn't have any neighbour*/
	//syntaxTree->neighbour = NULL;

	///*declare the node as statement*/
	/*syntaxTree->isStatement = 0;*/

	return init_SyntaxTree(terminal, {}, NULL, 0);

	//return syntaxTree;
}

//function to parse an exp by the rule 
// exp -> simple-exp [comparison-op simple-exp]
SyntaxTree_T* parse_exp(Parser_T* parser, SyntaxTree_T* syntaxTree)
{
	// /*initalize a syntaxTree*/
	SyntaxTree_T* calc_syntaxtree = new SyntaxTree_T();

	// /*make the parentNode => exp*/
	// syntaxTree->Node_name = "exp";


	// /*node dosn't have any neighbour*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as statement*/
	// syntaxTree->isStatement = 0;

	/*parse the simple-exp by making it a child node*/


	if ((parser->next_Token == NULL) )
	{
		syntaxTree = parse_SimpleExp(parser, syntaxTree);
		//parse_SimpleExp(parser, syntaxTree);
		if (syntaxTree == NULL)
		{
			return NULL;
		}


		return syntaxTree;
	}


	calc_syntaxtree = parse_SimpleExp(parser, calc_syntaxtree);
	//parse_SimpleExp(parser, syntaxTree);
	if (calc_syntaxtree == NULL)
	{
		return NULL;
	}


	//syntaxTree->children.push_back(syntaxTree_check);


	/*check if there is comparison-op*/
	if ((parser->next_Token != NULL) && (parser->current_Token->token_name == LESSTHAN_TOKEN || parser->current_Token->token_name == EQUAL_TOKEN))
	{
		/*parse the comparison-op by making it a child node*/
		calc_syntaxtree = parse_ComparisonOp(parser, calc_syntaxtree);
		if (calc_syntaxtree == NULL)
		{
			return NULL;
		}

		//syntaxTree->children.push_back(syntaxTree_check);

		/*parse the simple-exp by making it a child node*/
		//syntaxTree->children.push_back(parse_SimpleExp(parser,syntaxTree));
		calc_syntaxtree = parse_SimpleExp(parser, calc_syntaxtree);
		//parse_SimpleExp(parser, syntaxTree);
		if (calc_syntaxtree == NULL)
			return NULL;


	}
	else
	{
		copy_childrens(calc_syntaxtree, syntaxTree);
		return syntaxTree;

	}
	syntaxTree->children.push_back(calc_syntaxtree);

	return syntaxTree;
}

//function to parse an comparison-op by the rule 
// comparison -> < | =
SyntaxTree_T* parse_ComparisonOp(Parser_T* parser, SyntaxTree_T* syntaxTree) {

	/*initalize a syntaxTree*/
	syntaxTree->Node_name = "op\n(" + parser->current_Token->token_value + ")";


	/*make the parentNode => comparison-op*/
	//syntaxTree->Node_name = parse_terminal("op\n("+parser->current_Token->token_value+")") ;

	/*parse the terminal (< | =) by making it a child node*/
	//syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));

	/*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;

	/*expect either => (|number|identifier*/
	/*std::vector<TOKEN_NAMES> token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };

	advance_parser(parser, token_names);*/


	std::vector<TOKEN_NAMES> token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };

	/*advance the parser*/
	if (!advance_parser(parser, token_names,1))
		return NULL;

	return syntaxTree;
}

//function to parse a simple-exp by the rule 
// simple-exp -> term{addop term}
SyntaxTree_T* parse_SimpleExp(Parser_T* parser, SyntaxTree_T* syntaxTree) {

	// /*initalize a syntaxTree*/
	SyntaxTree_T* calc_syntaxtree = new SyntaxTree_T();
	SyntaxTree_T* round_syntaxtree = new SyntaxTree_T();

	// /*make the parentNode => simple-exp*/
	// syntaxTree->Node_name = "simple-exp";

	// /*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;

	int first_round = 1;

	/*if (parser->next_Token != NULL)
	{*/

	if ((parser->next_Token == NULL) )
	{
		syntaxTree = parse_term(parser, syntaxTree);
		//parse_SimpleExp(parser, syntaxTree);
		if (syntaxTree == NULL)
		{
			return NULL;
		}
		return syntaxTree;
	}

	/*make the child => term*/
	calc_syntaxtree = parse_term(parser, calc_syntaxtree);
	//parse_term(parser, syntaxTree);
	if (calc_syntaxtree == NULL)
	{
		return NULL;
	}

	//syntaxTree->children.push_back(syntaxTree_check);

	/*check if there is addop*/
	if((parser->next_Token != NULL) && (parser->current_Token->token_name == PLUS_TOKEN || parser->current_Token->token_name == MINUS_TOKEN))
	{
		while ((parser->next_Token != NULL) && (parser->current_Token->token_name == PLUS_TOKEN || parser->current_Token->token_name == MINUS_TOKEN))
		{
			if (!first_round)
			{
				round_syntaxtree->children.push_back(calc_syntaxtree);
				calc_syntaxtree = round_syntaxtree;
			}
			/*parse the addop by making it a child node*/
			calc_syntaxtree = parse_AddOp(parser, calc_syntaxtree);
			if (calc_syntaxtree == NULL)
			{
				return NULL;
			}
			//syntaxTree->children.push_back(syntaxTree_check);

			/*parse the term by making it a child node*/
			//syntaxTree->children.push_back(parse_term(parser));
			calc_syntaxtree = parse_term(parser, calc_syntaxtree);
			//parse_term(parser, syntaxTree);
			if (calc_syntaxtree == NULL)
				return NULL;

			first_round = 0;
		}
	}
	else
	{
		copy_childrens(calc_syntaxtree, syntaxTree);
		return syntaxTree;

	}

	 

	syntaxTree->children.push_back(calc_syntaxtree);

	return syntaxTree;
}

//function to parse an add-op by the rule 
// add-op -> +|-
SyntaxTree_T* parse_AddOp(Parser_T* parser, SyntaxTree_T* syntaxTree)
{
	/*initalize a syntaxTree*/
	syntaxTree->Node_name = "op\n(" + parser->current_Token->token_value + ")";

	// /*make the parentNode => simple-exp*/
	// syntaxTree->Node_name = "addop";

	// /*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;


	// /*make the child => terminal (+|-)*/
	// syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));

	/*expect either => (|number|identifier*/
	std::vector<TOKEN_NAMES> token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };

	/*advance the parser*/
	if (!advance_parser(parser, token_names,1))
		return NULL;

	return syntaxTree;


}

//function to parse a term by the rule 
// term ->factor {mulop factor}
SyntaxTree_T* parse_term(Parser_T* parser, SyntaxTree_T* syntaxTree)
{
	// /*initalize a syntaxTree*/
	// SyntaxTree_T* syntaxTree = new SyntaxTree_T();
	SyntaxTree_T* round_syntaxtree = new SyntaxTree_T();
	SyntaxTree_T* calc_syntaxtree = new SyntaxTree_T();

	// /*make the parentNode => term*/
	// syntaxTree->Node_name = "term";

	// /*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;

	int first_round = 1;

	/*if (parser->next_Token != NULL)
	{*/

	if ((parser->next_Token == NULL))
	{
		syntaxTree = parse_factor(parser, syntaxTree);
		//parse_SimpleExp(parser, syntaxTree);
		if (syntaxTree == NULL)
		{
			return NULL;
		}
		return syntaxTree;
	}

	/*make the child => factor*/
	calc_syntaxtree = parse_factor(parser, calc_syntaxtree);
	//parse_factor(parser, syntaxTree);
	if (calc_syntaxtree == NULL)
	{
		return NULL;
	}
	//syntaxTree->children.push_back(syntaxTree_check);

	/*check if there is addop*/
	if ((parser->next_Token != NULL) && (parser->current_Token->token_name == MULT_TOKEN || parser->current_Token->token_name == DIV_TOKEN))
	{
		while ((parser->next_Token != NULL) && (parser->current_Token->token_name == MULT_TOKEN || parser->current_Token->token_name == DIV_TOKEN))
		{
			if (!first_round)
			{
				round_syntaxtree->children.push_back(calc_syntaxtree);
				calc_syntaxtree = round_syntaxtree;
			}

			/*parse the mulop by making it a child node*/
			calc_syntaxtree = parse_MulOp(parser, calc_syntaxtree);
			if (calc_syntaxtree == NULL)
			{
				return NULL;
			}
			//syntaxTree->children.push_back(syntaxTree_check);

			/*parse the factor by making it a child node*/
			//syntaxTree->children.push_back(parse_factor(parser));
			calc_syntaxtree = parse_factor(parser, calc_syntaxtree);
			//parse_factor(parser, syntaxTree);
			if (calc_syntaxtree == NULL)
				return NULL;

			first_round = 0;
		}
	}
	else
	{
		copy_childrens(calc_syntaxtree, syntaxTree);
		return syntaxTree;

	}
	syntaxTree->children.push_back(calc_syntaxtree);



	return syntaxTree;
}

//function to parse an mulop by the rule 
// mulop -> * | /
SyntaxTree_T* parse_MulOp(Parser_T* parser, SyntaxTree_T* syntaxTree)
{
	/*initalize a syntaxTree*/
	syntaxTree->Node_name = "op\n(" + parser->current_Token->token_value + ")";

	// /*make the parentNode => simple-exp*/
	// syntaxTree->Node_name = "mulop";

	// /*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;


	// /*make the child => terminal (*|\)*/
	// syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));

	/*expect either => (|number|identifier*/
	std::vector<TOKEN_NAMES> token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };

	/*advance the parser*/
	if (!advance_parser(parser, token_names,1))
		return NULL;

	return syntaxTree;
}

//function to parse a factor by the rule 
// factor -> (exp) | number | identifier
SyntaxTree_T* parse_factor(Parser_T* parser, SyntaxTree_T* syntaxTree)
{

	std::vector<TOKEN_NAMES> token_names;

	/*initalize a syntaxTree*/
	//SyntaxTree_T* syntaxTree = new SyntaxTree_T();

	///*make the parentNode => simple-exp*/
	//syntaxTree->Node_name = "factor";

	///*initialize other valuse*/
	//syntaxTree->neighbour = NULL;

	///*declare the node as exp*/
	//syntaxTree->isStatement = 0;

	if (parser->current_Token->token_name == OPENBRACKET_TOKEN)
	{
		/*parse the openBracket('(') by making it a child node*/
		parse_terminal(parser->current_Token->token_value);
		//syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));

		/*expect either => (|number|identifier*/
		token_names = { OPENBRACKET_TOKEN ,NUMBER_TOKEN ,IDENTIFIER_TOKEN };

		/*advance the parser*/
		if (!advance_parser(parser, token_names,1))
			return NULL;

		/*parse the mulop by making it a child node*/
		//syntaxTree->children.push_back(parse_exp(parser,syntaxTree));
		syntaxTree = parse_exp(parser, syntaxTree);
		//parse_exp(parser, syntaxTree);
		if (syntaxTree == NULL)
		{
			return NULL;
		}

		/*parse the closeBracket(')') by making it a child node*/
		parse_terminal(parser->current_Token->token_value);
		//syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));

		//if (parser->current_index_in_Tokens_Vector + 1 == Tokens_Vector.size() - 1)
		//{
		//	/*if we reached here it means the parsing ended*/
		//	parser->current_index_in_Tokens_Vector++;
		//	parser->current_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector];
		//	parser->next_Token = NULL;

		//}
		if (parser->next_Token != NULL)
		{
			/*EXPECTED AFTER (')')*/
			std::vector<TOKEN_NAMES> token_names = { SEMICOLON_TOKEN,PLUS_TOKEN,MINUS_TOKEN,LESSTHAN_TOKEN,MULT_TOKEN ,
				DIV_TOKEN ,IDENTIFIER_TOKEN,CLOSEBRACKET_TOKEN,THEN_TOKEN,
				REPEAT_TOKEN,ASSIGN_TOKEN,WRITE_TOKEN,END_TOKEN,ELSE_TOKEN,UNTIL_TOKEN };

			/*advance the parser*/
			if (!advance_parser(parser, token_names,0))
				return NULL;

		}
	}

	else if (parser->current_Token->token_name == NUMBER_TOKEN)
	{

		

		/*parse the number by making it a child node*/
		SyntaxTree_T* syntaxTree_check = parse_number(parser);
		if (syntaxTree_check == NULL)
		{
			return NULL;
		}
		syntaxTree->children.push_back(syntaxTree_check);

	}
	else if (parser->current_Token->token_name == IDENTIFIER_TOKEN)
	{
		
		/*parse the identifier by making it a child node*/
		SyntaxTree_T* syntaxTree_check = parse_identifier(parser);
		if (syntaxTree_check == NULL)
		{
			return NULL;
		}
		syntaxTree->children.push_back(syntaxTree_check);

	}

	return syntaxTree;
}

//function to parse a number by the rule 
// number -> is any number
SyntaxTree_T* parse_number(Parser_T* parser)
{
	/*initalize a syntaxTree*/
	SyntaxTree_T* syntaxTree = parse_terminal("const\n(" + parser->current_Token->token_value + ")");

	// /*make the parentNode => simple-exp*/
	// syntaxTree->Node_name = "number";

	// /*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;


	// /*make the child => terminal (*|\)*/
	// syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));

	//if (parser->current_index_in_Tokens_Vector + 1 == Tokens_Vector.size() - 1)
	//{
	//	/*if we reached here it means the parsing ended*/
	//	parser->current_index_in_Tokens_Vector++;
	//	parser->current_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector];
	//	parser->next_Token = NULL;

	//}
	if (parser->next_Token != NULL)
	{
		/*EXPECTED AFTER NUMBER*/
		std::vector<TOKEN_NAMES> token_names = { SEMICOLON_TOKEN,PLUS_TOKEN,MINUS_TOKEN,EQUAL_TOKEN,LESSTHAN_TOKEN,MULT_TOKEN ,
				DIV_TOKEN ,THEN_TOKEN,
				REPEAT_TOKEN,ASSIGN_TOKEN,WRITE_TOKEN,END_TOKEN,ELSE_TOKEN,UNTIL_TOKEN,CLOSEBRACKET_TOKEN };
		//std::vector<TOKEN_NAMES> token_names = { NUMBER_TOKEN };

		/*advance the parser*/
		if (!advance_parser(parser, token_names,0))
			return NULL;
	}

	return syntaxTree;
}

//function to parse an indentifier by the rule 
// identifier -> is any identifier
SyntaxTree_T* parse_identifier(Parser_T* parser)
{
	/*initalize a syntaxTree*/
	SyntaxTree_T* syntaxTree = parse_terminal("id\n(" + parser->current_Token->token_value + ")");

	// /*make the parentNode => simple-exp*/
	// syntaxTree->Node_name = "identifier";

	// /*initialize other valuse*/
	// syntaxTree->neighbour = NULL;

	// /*declare the node as exp*/
	// syntaxTree->isStatement = 0;

	// /*make the child => terminal (*|\)*/
	// syntaxTree->children.push_back(parse_terminal(parser->current_Token->token_value));


	//if (parser->current_index_in_Tokens_Vector + 1 == Tokens_Vector.size() - 1)
	//{
	//	/*if we reached here it means the parsing ended*/
	//	parser->current_index_in_Tokens_Vector++;
	//	parser->current_Token = Tokens_Vector[parser->current_index_in_Tokens_Vector];
	//	parser->next_Token = NULL;

	//}
	if (parser->next_Token != NULL)
	{
		/*EXPECTED TOKEN AFTER IDENTIFIER*/
		std::vector<TOKEN_NAMES> token_names = { SEMICOLON_TOKEN,PLUS_TOKEN,MINUS_TOKEN,EQUAL_TOKEN,LESSTHAN_TOKEN,MULT_TOKEN ,
				DIV_TOKEN ,THEN_TOKEN,
				REPEAT_TOKEN,ASSIGN_TOKEN,WRITE_TOKEN,END_TOKEN,ELSE_TOKEN,UNTIL_TOKEN,CLOSEBRACKET_TOKEN };

		//std::vector<TOKEN_NAMES> token_names = { IDENTIFIER_TOKEN };

		/*advance the parser*/
		if (!advance_parser(parser, token_names,0))
			return NULL;
	}

	return syntaxTree;
}



// function that read an input file containing Tiny language syntax 
// to generate a vector containing 
// // all tokens in the file
vector<Token_T*> scanner_get_all_Tokens(string input_as_string)
{
	/*declaring variables*/
//    ifstream input_file;
    vector<Token_T*> TokenVector;

//	/*reading input file name*/
//	input_file.open(input_file_path, fstream::in);
//	stringstream buffer;
//	buffer << input_file.rdbuf();
//	string input_as_string(buffer.str());
//	input_file.close();

	/*scanning input string*/
	Scanner_T* lexer = init_scanner(input_as_string);
	Token_T* token = NULL;


	/*storing generated tokens in a TokenVector*/
	while ((token = scanner_get_next_token(lexer)) != NULL)
	{
		if (token->token_name == INVALID_TOKEN)
		{
			/*if this happend then we reached an error*/
			error_message = "INVALID TOKEN => " + token->token_value;
			is_error = 1;
			return TokenVector;
		}

		TokenVector.push_back(token);

	}

	return TokenVector;
}

/*************************Function to draw the syntax tree*************************/
 int counter = 1;
 int drawSyntaxTree(SyntaxTree_T* syntaxTree, string &str, int id) {
    int edge;
    //SyntaxTree_T* syntaxTree = syntaxTree_Vector[0];
    /* Check whether the node is a statement or an expression */
    if (syntaxTree->isStatement) {
        str += "node[shape = rect label = \"" + syntaxTree->Node_name + "\"]\nid" + to_string(id) + "\n";
    } else {
        str += "node[shape = oval label = \"" + syntaxTree->Node_name + "\"]\nid" + to_string(id) + "\n";
    }
    //---------------To order children from left to right--------------------
    if ((int)syntaxTree->children.size() > 0) {
        str += "node[ordering = \"out\"]\n";
    } else {
        str += "node[ordering = \"in\"]\n";
    }
    //-----------------------------------------------------------------------

    /* Drawing the node children if any */
    for (int i = 0; i < (int)syntaxTree->children.size(); i++) {
        counter++;
        edge = drawSyntaxTree(syntaxTree->children[i], str, counter);
        str += "id" + to_string(id) + "--id" + to_string(edge) + "\n";
    }

    if (syntaxTree->neighbour != NULL) {
        counter++;
        edge = drawSyntaxTree(syntaxTree->neighbour, str, counter);
        str += "{rank = same; id" + to_string(id) + "; id" + to_string(edge) + "}\n";
        str += "id" + to_string(id) + "--id" + to_string(edge) + "\n";
    }

    return id;
 }

/************************Convert from string to file********************************/
 void qstringToFile(QString qstring) {
     QString filename = "dotLang.dot";
     QFile file(filename);
     QFile::remove(filename);
     if (file.open(QIODevice::ReadWrite)) {
             QTextStream stream(&file);
             stream << qstring;
     }
 }

/*********************************Main_Funciton*************************************/

//function to return all syntaxTree of the program
std::vector<SyntaxTree_T*> generate_programm_tree(std::string input_file_path)
{
	is_error = 0;
	/*initialize the vector that contain all the statements syntacs Tree*/
	std::vector<SyntaxTree_T*> syntaxTree_Vector;

	/*initialize the syntaxTree*/
	SyntaxTree_T* syntaxTree;
	/*get all the tokens in the program*/
	//std::vector<Token_T*> Tokens_Vector = scanner_get_all_Tokens(input_file_path);
	Tokens_Vector = scanner_get_all_Tokens(input_file_path);
	current_line_number = 1;
	/*Then there is no INVALID TOKEN*/
	if (!is_error)
	{
		Parser_T* parser = init_Parser(0);
		//globalParser = init_Parser(0);

		while (parser->next_Token != NULL)
		{
			syntaxTree = parse_program(parser);
			if (syntaxTree == NULL)
			{
				/*then there is an error*/

				syntaxTree_Vector.push_back(NULL);
                //cout << "ERROR: " << Error_message->error_message << endl;
                //cout << "Error string: " << error_message << endl;
				//ERROR_T* Error_message = init_Error(error_message, is_error);
				return syntaxTree_Vector;

			}
			else
			{
				/*if we reached here then there is not error in the current statement and it parsed correctly*/

				/*push the generated current tree in the vector<SyntaxTree_T*>*/
				syntaxTree_Vector.push_back(syntaxTree);

				/*Then we advance to the next line*/
				current_line_number += current_line_number;
			}
		}

	}
	else
	{
		/*if we reached here Then there is Invalid Token*/
		syntaxTree_Vector.push_back(NULL);
		ERROR_T* Error_message = init_Error(error_message, is_error);
        cout << "ERROR: " << Error_message->error_message << endl;
        cout << "Error string: " << error_message << endl;
		return syntaxTree_Vector;
	}
	/*if we reached here then the program parsed correctly => there is no error*/
	ERROR_T* Error_message = init_Error("", 0);

    cout << "ERROR: " << Error_message->error_message << endl;
    cout << "Error string: " << error_message << endl;
    //error_msg = Error_message->error_message;
    /* Draw the syntax tree */
    syntaxTree = syntaxTree_Vector[0];
    string str = "graph main {";
    //cout << "before drawSyntaxTree" << endl;
    int edge = drawSyntaxTree(syntaxTree, str, 1);
    //cout << "after drawSyntaxTree" << endl;
    str += "}";
    cout << str << endl;
    qstringToFile(QString::fromStdString(str));
    QString cpath = QDir::currentPath();
    QDir::setCurrent(cpath);
    //cout << "before system function" << endl;
    system("dot -Tpng -O dotLang.dot");
    //cout << "after system function" << endl;

	return syntaxTree_Vector;
}

//void printError(string error_m, int is_er) {
//    cout << "ERROR FUNC: " << error_m << endl;
//}
//int main()
//{
//	vector<SyntaxTree_T*> tree_vector = generate_programm_tree("E:\\input_file2.txt");

//	cout << error_message << endl;
//	int a = 1;

//	return 0;
//}
