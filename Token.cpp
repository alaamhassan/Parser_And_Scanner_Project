#include "TOKEN.h"
#include <cstdlib>
#include<string>

//intialize the token by:
//assigning location in memory
//initializ it's contents
Token_T* init_token(TOKEN_NAMES token_name, std::string token_value) {

	Token_T* token = new Token_T();

	token->token_name = token_name;

	token->token_value = token_value;

	return token;

}

