#pragma once

namespace dtack
{
	namespace base
	{

	  // --------------------------------------------------------------------
	  class string_walker_c
	  {
	private:
		std::string b;
		int line = 1;
		int psol = 1;
	public:
		int p = 0;
	private:
		int l = 0;
	public:
		virtual ~string_walker_c()
		{
		}

		string_walker_c(std::string b);

		// --------------------------------------------------------------------
		int peek_line();

		// --------------------------------------------------------------------
		int peek_psol();

		// --------------------------------------------------------------------
		int peek_p();

		// --------------------------------------------------------------------
		bool is_exhausted();

		// --------------------------------------------------------------------
		void back_up(int n);

		// --------------------------------------------------------------------
		bool is_whitespace();

		// --------------------------------------------------------------------
		STRING_TOKEN::TYPE detect_token_type(string_token_c *token);

	  }; // end class

	}
} // end namespace
