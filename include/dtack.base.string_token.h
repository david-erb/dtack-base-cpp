#pragma once

namespace dtack
{
	namespace base
	{
	  class STRING_TOKEN
	  {
	public:
		enum class TYPE
		{
		  UNSET,
		  START,
		  EXHAUSTED,
		  WHITE_SPACE,
		  BACKSLASH,
		  COLON,
		  COMMA,
		  LEFT_CURLY_BRACKET,
		  RIGHT_CURLY_BRACKET,
		  LEFT_SQUARE_BRACKET,
		  RIGHT_SQUARE_BRACKET,
		  LITERAL,
		  PACKET,
		  OTHER
		};
	  };

	  // --------------------------------------------------------------------
	  class string_token_c
	  {
	public:
		std::string text = "";
		STRING_TOKEN::TYPE type = STRING_TOKEN::TYPE::UNSET;
		int p = 0;

		string_token_c();

		string_token_c(const std::string &text, STRING_TOKEN::TYPE type, int p);

        std::string to_string()
        {
            std::string label(c_str());
            return label + (text == ""? "": " \"" + text + "\"");
        }
        
        const char *c_str()
        {
            switch(type)
            {
                case STRING_TOKEN::TYPE::UNSET: return "UNSET";
                case STRING_TOKEN::TYPE::START: return "START";
                case STRING_TOKEN::TYPE::EXHAUSTED: return "EXHAUSTED";
                case STRING_TOKEN::TYPE::WHITE_SPACE: return "WHITE_SPACE";
                case STRING_TOKEN::TYPE::BACKSLASH: return "BACKSLASH";
                case STRING_TOKEN::TYPE::COLON: return "COLON";
                case STRING_TOKEN::TYPE::COMMA: return "COMMA";
                case STRING_TOKEN::TYPE::LEFT_CURLY_BRACKET: return "LEFT_CURLY_BRACKET";
                case STRING_TOKEN::TYPE::RIGHT_CURLY_BRACKET: return "RIGHT_CURLY_BRACKET";
                case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET: return "LEFT_SQUARE_BRACKET";
                case STRING_TOKEN::TYPE::RIGHT_SQUARE_BRACKET: return "RIGHT_SQUARE_BRACKET";
                case STRING_TOKEN::TYPE::LITERAL: return "LITERAL";
                case STRING_TOKEN::TYPE::PACKET: return "PACKET";
                case STRING_TOKEN::TYPE::OTHER: return "OTHER";
                default: return text.c_str();
            }
        } // end method

	  }; // end class


	}
} // end namespace
