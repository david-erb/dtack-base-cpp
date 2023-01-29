#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "dtack.base.string_token.h"
#include "dtack.base.string_walker.h"
#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.packet.h"
using namespace dtack::base;

namespace dtack
{
	namespace base
	{
        // --------------------------------------------------------------------
		std::string packet_c::compose_jso_value(const std::string &source)
		{
          std::string s = source;
									   // need to escape backslashes when composing jso string from packet
									   // watchfrog #180
		  s = replace(s, std::string("\\"), std::string("\\") + "\\");
		  s = replace(s, std::string("\""), std::string("\\") + "\"");

									   // do proper escaping of newline characters in jso structure 
									   // watchfrog #248
		  s = replace(s, std::string("\r"), std::string("\\") + "r");
		  s = replace(s, std::string("\n"), std::string("\\") + "n");
		  return s;
		}

        // --------------------------------------------------------------------
		void packet_c::compose_jso_line(std::vector<std::string> &lines, int nest_level, const std::string &line)
		{
		  if (nest_level > 0)
		  {
			if ((int)indent.length() >= nest_level * 2)
			{
			  lines.push_back(indent.substr(0, nest_level * 2) + line);
			}
			else
			{
			  lines.push_back(indent + line);
			}
		  }
		  else
		  {
			lines.push_back(line);
		  }
		}

        // --------------------------------------------------------------------
		void packet_c::compose_jso_lines(std::vector<std::string> &lines, int nest_level)
		{

		  compose_jso_line(lines, nest_level, "\"" + this->name + "\": {");

		  nest_level++;

		  if (attribs.size() > 0)
		  {
			std::string last_attrib = "";
			for (auto attrib : attribs)
			{
			  last_attrib = attrib.first;
			}

			compose_jso_line(lines, nest_level, "\"attributes\": {");
			for (auto attrib : attribs)
			{
									   // fix bug when making jso from packet attributes
									   // watchfrog #177
			  compose_jso_line(lines, nest_level, "\"" + attrib.first + "\": \"" + compose_jso_value(attribs[attrib.first]) + "\"" + (attrib.first == last_attrib? "": ","));
			}
			compose_jso_line(lines, nest_level, std::string("}") + (scalars.size() > 0 || packets.size() > 0 ? "," : ""));
		  }


		  if (scalars.size() > 0)
		  {
			std::string last_scalar = "";
			for (auto scalar : scalars)
			{
			  last_scalar = scalar.first;
			}

			for (auto scalar : scalars)
			{
			  compose_jso_line(lines, nest_level, "\"" + scalar.first + "\": \"" + compose_jso_value(scalars[scalar.first]) + "\"" + (scalar.first != last_scalar || packets.size() > 0 ? "," : ""));
			}
		  }

		  if (packets.size() > 0)
		  {
			unsigned int n = 0;
			for (auto packet : packets)
			{
			  packet->compose_jso_lines(lines, nest_level);
			  n++;
										// need commas between packets
										// watchfrog #138
			  if (n < packets.size())
			  {
				compose_jso_line(lines, nest_level, ",");
			  }
			}
		  }

		  compose_jso_line(lines, nest_level, "}");

		  nest_level--;
		}

        // --------------------------------------------------------------------
		std::string packet_c::compose_jso_string()
		{
		  std::string s;

		  compose_jso_string(s);

		  return s;
		}

		void packet_c::compose_jso_string(std::string &s)
		{
		  std::vector<std::string> lines;

										// watchfrog #144
		  int nest_level = -10000000;

		  bool should_indent = base_c::value(options, "indent", "no") == "yes";

		  if (should_indent)
		  {
			nest_level = 0;
		  }

		  compose_jso_lines(lines, nest_level);

		  for (auto line : lines)
		  {
			s += line;
			if (should_indent)
				s += "\n";
		  }
		} // end method

	} // end namespace
    
} // end namespace
