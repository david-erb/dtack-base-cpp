#include <string>
#include <sstream>

#include "string_builder.h"
#include "dtack.base.string_token.h"
#include "dtack.base.string_walker.h"

namespace dtack
{
	namespace base
	{

		string_walker_c::string_walker_c(std::string b)
		{
		  this->b = b;
		  p = 0;
		  l = b.length();
		} // end constructor

		int string_walker_c::peek_line()
		{
		  return line;
		}

		int string_walker_c::peek_psol()
		{
		  return p - psol;
		}

		int string_walker_c::peek_p()
		{
		  return p;
		}

		bool string_walker_c::is_exhausted()
		{
		  return p >= l;
		} // end method

		void string_walker_c::back_up(int n)
		{
		  p -= n;
		  if (p < 0)
		  {
			p = 0;
		  }
		} // end method

		bool string_walker_c::is_whitespace()
		{
		  if (b[p] == L'\r' && p < l - 1 && b[p + 1] != L'\n')
		  {
			psol = p;
			line++;
		  }
		  else
		  {
		  if (b[p] == L'\n')
		  {
			psol = p;
			line++;
		  }
		  }

		  return b[p] == L'\n' || b[p] == L'\r' || b[p] == L'\t' || b[p] == L' ';
		} // end method

		STRING_TOKEN::TYPE string_walker_c::detect_token_type(string_token_c *token)
		{
		  token->text = "";

		  if (is_exhausted())
		  {
			return token->type = STRING_TOKEN::TYPE::EXHAUSTED;
		  }

		  if (is_whitespace())
		  {
			p++;
			while (p < l && is_whitespace())
			{
			  p++;
			}
			return token->type = STRING_TOKEN::TYPE::WHITE_SPACE;
		  }

		  token->p = p;

		  char c = b[p++];

		  if (c == L'\\')
		  {
			return token->type = STRING_TOKEN::TYPE::BACKSLASH;
		  }

		  if (c == L':')
		  {
			return token->type = STRING_TOKEN::TYPE::COLON;
		  }

		  if (c == L',')
		  {
			return token->type = STRING_TOKEN::TYPE::COMMA;
		  }

		  if (c == L'{')
		  {
			return token->type = STRING_TOKEN::TYPE::LEFT_CURLY_BRACKET;
		  }

		  if (c == L'}')
		  {
			return token->type = STRING_TOKEN::TYPE::RIGHT_CURLY_BRACKET;
		  }

		  if (c == L'[')
		  {
			return token->type = STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET;
		  }

		  if (c == L']')
		  {
			return token->type = STRING_TOKEN::TYPE::RIGHT_SQUARE_BRACKET;
		  }

		  if (c == L'"')
		  {
			int p1 = p;
			while (p < l && b[p] != L'"')
			{
											// fix problem when json literal has an escaped double-quote in it
			  if (b[p] == L'\\')
			  {
				p++;
			  }
			  if (p < l)
			  {
				p++;
			  }
			}

			token->text = b.substr(p1, p - p1);

			if (p < l)
			{
			  p++;
			}

			return token->type = STRING_TOKEN::TYPE::LITERAL;
		  }

		  if (c == L'-' || (c >= L'0' && c <= L'9'))
		  {
			int p1 = p - 1;
			while (p < l && (b[p] == L'.' || (b[p] >= L'0' && b[p] <= L'9')))
			{
			  p++;
			}

											// eztask #15770 PSS parse fails on exponent literal
			if (p < l && (b[p] == L'e' || b[p] == L'E'))
			{
			  p++;
			  if (p < l && b[p] == L'-')
			  {
				p++;
			  }

			  while (p < l && (b[p] >= L'0' && b[p] <= L'9'))
			  {
				p++;
			  }
			}

			token->text = b.substr(p1, p - p1);

			return token->type = STRING_TOKEN::TYPE::LITERAL;
		  }


		  {
			int p1 = p - 1;
			while (p < l && !is_whitespace() && b[p] != L':' && b[p] != L',' && b[p] != L'{' && b[p] != L'}' && b[p] != L'[' && b[p] != L']' && b[p] != L'"')
			{
			  p++;
			}

			token->text = b.substr(p1, p - p1);

			return token->type = STRING_TOKEN::TYPE::LITERAL;
		  }

		  return token->type = STRING_TOKEN::TYPE::OTHER;

		} // end method
	}
}
