#include <string>
#include "dtack.base.string_token.h"
namespace dtack
{
	namespace base
	{

		string_token_c::string_token_c()
		{
		}

		string_token_c::string_token_c(const std::string &text, STRING_TOKEN::TYPE type, int p)
		{
		  this->text = text;
		  this->type = type;
		  this->p = p;
		}
	}
}
