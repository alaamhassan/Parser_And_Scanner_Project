#include "ERROR.h"

ERROR_T* init_Error(std::string error_message, bool is_error)
{
	ERROR_T* Error = new ERROR_T();

	Error->error_message = error_message;
	Error->is_error = is_error;

	return Error;
}