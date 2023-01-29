#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>
#include <unordered_map>
#include "dtack.base.string_token.h"
#include "dtack.base.string_walker.h"
#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.packet.h"

namespace dtack
{
	namespace base
	{
        // --------------------------------------------------------------------
		void packet_c::load_from_json_filename(const std::string &filename)
		{
			FILE *file = fopen(filename.c_str(), "r");

			if (file)
			{
				fclose(file);
				std::ifstream stream(filename.c_str());
				std::stringstream buffer;
				buffer << stream.rdbuf();                    
				try
				{
					// Parse the file's content.
					load_from_json_string(buffer.str());
				}
				catch(std::exception &exception)
				{
					throw_runtime_error("unable to parse json file: %s", exception.what());
				}
			}
			else
			{
				throw_runtime_error("unable to open json file %s", filename.c_str());
			}


		} // end method


        // --------------------------------------------------------------------
		void packet_c::load_from_json_string(const std::string &b)
		{
		  std::shared_ptr<string_walker_c> string_walker = std::make_shared<string_walker_c>(b);

		  load_from_json_string_walker(string_walker);

		} // end method

        // --------------------------------------------------------------------
		void packet_c::load_from_json_string_walker(
			std::shared_ptr<string_walker_c> string_walker)
		{
		  std::shared_ptr<string_token_c> had_token = std::make_shared<string_token_c>("", STRING_TOKEN::TYPE::START, 0);

		  load_from_json_string_walker(string_walker, had_token);

		} // end method

        // --------------------------------------------------------------------
		void packet_c::load_from_json_string_walker(
			std::shared_ptr<string_walker_c> string_walker, 
			std::shared_ptr<string_token_c> had_token)
		{
		  this->string_walker = string_walker;

		  std::shared_ptr<string_token_c> originally_had_token = had_token;

		  should_quit = false;

		  while (!should_quit && !string_walker->is_exhausted())
		  {
			std::shared_ptr<string_token_c> have_token = std::make_shared<string_token_c>();

			string_walker->detect_token_type(have_token.get());

            // printf("%s had token %s have token %s\n", __func__, had_token->to_string().c_str(),
            //   have_token->to_string().c_str());

			switch (have_token->type)
			{
			  case STRING_TOKEN::TYPE::EXHAUSTED:
				handle_exhausted(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::WHITE_SPACE:
				continue;
			  break;
			  case STRING_TOKEN::TYPE::COLON:
				handle_colon(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::COMMA:
				handle_comma(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::LEFT_CURLY_BRACKET:
				handle_left_curly_bracket(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::RIGHT_CURLY_BRACKET:
				handle_right_curly_bracket(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET:
				handle_left_square_bracket(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::RIGHT_SQUARE_BRACKET:
				handle_right_square_bracket(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::LITERAL:
				handle_literal(have_token, had_token);
			  break;
			  case STRING_TOKEN::TYPE::OTHER:
				handle_other(have_token, had_token);
			  break;
              default:
				handle_other(have_token, had_token);
			  break;
			}

			had_token = have_token;
		  }

			  originally_had_token->text = name;
			  originally_had_token->type = STRING_TOKEN::TYPE::PACKET;


		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_something(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::COLON:
			  raise_unexpected_after(have_token, had_token);
			break;
			case STRING_TOKEN::TYPE::COMMA:
			  raise_unexpected_after(have_token, had_token);
			break;
			case STRING_TOKEN::TYPE::LEFT_CURLY_BRACKET:
			  raise_unexpected_after(have_token, had_token);
			break;
			case STRING_TOKEN::TYPE::RIGHT_CURLY_BRACKET:
			break;
			case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET:
			  raise_unexpected_after(have_token, had_token);
			break;
			case STRING_TOKEN::TYPE::RIGHT_SQUARE_BRACKET:
			  raise_unexpected_after(have_token, had_token);
			break;
			case STRING_TOKEN::TYPE::LITERAL:
			  raise_unexpected_after(have_token, had_token);
			break;
			case STRING_TOKEN::TYPE::OTHER:
			  handle_other(have_token, had_token);
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }
		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_exhausted(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_colon(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::LITERAL:
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_comma(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  //string F = enter("handle_comma");
		  //track(this, F, "got " + have_token.type + " in \"" + name + "\" after " + had_token.type + " \"" + had_token.text + "\"");

		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::PACKET:
			break;
			case STRING_TOKEN::TYPE::LITERAL:
			  //if (name.StartsWith("array"))
			  if (get_attribute("is_loading_json_array") == "yes")
			  {
				add_scalar(printf_to_string("item%08d", scalars.size()), had_token->text);
			  }
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_left_curly_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::START:
			break;
			case STRING_TOKEN::TYPE::COLON:
			case STRING_TOKEN::TYPE::COMMA:
			case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET:
			{
			  std::shared_ptr<packet_c> packet = this->new_packet(property_name_token == nullptr || property_name_token->text == ""? compose_object_name(): property_name_token->text);
			  property_name_token = nullptr;
			  packet->load_from_json_string_walker(string_walker, have_token);
			break;
			}
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }


		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_right_curly_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::PACKET:
			{
			  std::string this_name = this->name;
			  std::string this_location = compose_location();
			  should_quit = true;
			}
			break;

											// solve problem of parsing an empty json object
											// watchfrog #431
			case STRING_TOKEN::TYPE::LEFT_CURLY_BRACKET:
			{
											// not doing it this way leaves "something: {}" composing xml as <something></something>, ie. a scalar
											// when parsing xml written with this, you will need packet.parse_node_with_attribs_as_packet = true
			  add_attrib("PARSED_FROM_EMPTY_JSON_OBJECT", "yes");
			  std::string this_name = this->name;
			  std::string this_location = compose_location();
			  should_quit = true;
			}
			break;

			case STRING_TOKEN::TYPE::LITERAL:
			  should_quit = true;
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_left_square_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::COLON:
			{
			  std::shared_ptr<packet_c> packet = this->new_packet(property_name_token == nullptr || property_name_token->text == ""? compose_array_name(): property_name_token->text);
			  property_name_token = nullptr;
			  //string_walker.back_up(1);
			  std::string this_name = this->name;
			  std::string this_location = compose_location();
			  packet->set_attribute("is_loading_json_array", "yes");
			  packet->load_from_json_string_walker(string_walker, have_token);
			}
			break;
			case STRING_TOKEN::TYPE::COMMA:
			case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET:
			{
			  std::shared_ptr<packet_c> packet = this->new_packet(compose_array_name());
			  packet->set_attribute("is_loading_json_array", "yes");
			  packet->load_from_json_string_walker(string_walker, have_token);
			}
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_right_square_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::PACKET:
			{
			  std::string this_name = this->name;
			  std::string this_location = compose_location();
			  should_quit = true;
			}
			break;
			case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET:
			case STRING_TOKEN::TYPE::LITERAL:
			{
											// use the attribute mechanism to determine the last item of an array
											// watchfrog #428
			  //if (name.StartsWith("array"))
			  if (get_attribute("is_loading_json_array") == "yes")
			  {
											// fix problem where finding [] with nothing between them gave an error when composing
											// watchfrog #430
				if (had_token->text != "")
				{
				  add_scalar(printf_to_string("item%08d", scalars.size()), had_token->text);
				}
				else
				{
											// not doing it this way leaves "something: []" composing xml as <something></something>, ie. a scalar
											// when parsing xml written with this, you will need packet.parse_node_with_attribs_as_packet = true
				  add_attrib(std::string("PARSED_FROM_EMPTY_JSON_ARRAY"), std::string("yes"));
				}
			  }
			  std::string this_name = this->name;
			  std::string this_location = compose_location();
			  should_quit = true;
			}
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_literal(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			case STRING_TOKEN::TYPE::COLON:
              if (property_name_token == nullptr)
              {
			    raise_unexpected_after(have_token, had_token);
              }
			  add_scalar(property_name_token->text, have_token->text);
			  property_name_token = nullptr;
			break;
			case STRING_TOKEN::TYPE::COMMA:
			case STRING_TOKEN::TYPE::LEFT_CURLY_BRACKET:
			case STRING_TOKEN::TYPE::LEFT_SQUARE_BRACKET:
			  property_name_token = have_token;
			break;
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		void packet_c::handle_other(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  switch (had_token->type)
		  {
			default:
			  raise_unexpected_after(have_token, had_token);
			break;
		  }

		} // end method

        // --------------------------------------------------------------------
		std::string packet_c::compose_array_name()
		{
		  return "array_" + compose_location();
		} // end method

        // --------------------------------------------------------------------
		std::string packet_c::compose_object_name()
		{
		  return "object_" + compose_location();
		} // end method

        // --------------------------------------------------------------------
		std::string packet_c::compose_location()
		{
		  return std::to_string(string_walker->peek_line()) + "_" + std::to_string(string_walker->peek_psol() - 1);
		} // end method

        // --------------------------------------------------------------------
		void packet_c::raise_unexpected_after(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token)
		{
		  throw std::runtime_error(
              "unexpected token " + have_token->to_string() + 
              " following token " + had_token->to_string() + 
              " at position " + std::to_string(have_token->p));
		} // end method

        // --------------------------------------------------------------------
		void packet_c::legalize_names()
		{
		  for (auto packet : packets)
		  {
			if (packet->name != "")
			{
			  packet->name = replace(packet->name, "@", "___");
			}
		  }

		} // end method

	} // end namespace
    
} // end namespace
