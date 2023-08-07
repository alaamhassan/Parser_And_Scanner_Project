#ifndef ERROR_H
#define ERROR_H
#include <string>

typedef struct ERROR_STRUCT
{
	std::string error_message;
	bool is_error;
}ERROR_T;

ERROR_T* init_Error(std::string error_message, bool is_error);
#endif