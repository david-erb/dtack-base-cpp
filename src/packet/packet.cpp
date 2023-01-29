#include <string>
#include <cstring>
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

		packet_c::packet_c(environment_c *environment) : base_c(environment)
		{
		}

		packet_c::packet_c(environment_c *environment, const std::string &name) : base_c(environment)
		{
		   this->name = name;
		}

        // ----------------------------------------------------------------
        packet_c::~packet_c()
        {
            // const char* F = "~packet_c";
            // printf(".......... %s: destructing packet %s\n", F, name.c_str());
        }

        // ----------------------------------------------------------------
		std::string packet_c::compose_text_value(const std::string &value)
		{
		  return value;
		}

        // ----------------------------------------------------------------
		std::string packet_c::compose_text_scalar(const std::string &scalar_name)
		{
		  std::string value = scalars[scalar_name];
		  return compose_text_value(value);
		}

        // --------------------------------------------------------------------
		void packet_c::compose_text_line(std::vector<std::string> &lines, int nest_level, const std::string &line)
		{
		  if (nest_level >= 0)
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
		void packet_c::compose_text_lines(std::vector<std::string> &lines, int nest_level)
		{
		  std::string s = "";
		  for (auto attrib : attribs)
		  {
			s += " " + attrib.first + "=\"" + compose_text_value(attribs[attrib.first]) + "\"";
		  }

			compose_text_line(lines, nest_level, this->name + s);

			for (auto scalar : scalars)
			{
			  compose_text_line(lines, nest_level + 1, scalar.first + ": " + compose_text_scalar(scalar.first));
			}

			for (auto cdata : cdatas)
			{
			  compose_text_line(lines, nest_level, "cdata");
			  compose_text_line(lines, 0, cdata);
			}

			for (auto packet : packets)
			{
			  packet->compose_text_lines(lines, nest_level + 1);
			}
		}

        // --------------------------------------------------------------------
		void packet_c::compose_text_string(std::string &s)
		{
		  std::vector<std::string> lines;

		  int nest_level = -10000000;

		  if (base_c::value(options, "indent", "yes") == "yes")
		  {
			nest_level = 0;
		  }

		  compose_text_lines(lines, nest_level);

		  for (auto line : lines)
		  {
			s += line;
			s += "\n";
		  }
		}

        // --------------------------------------------------------------------
		std::string packet_c::peek_name()
		{
		   return name_attrib != ""? base_c::require(attribs, name_attrib): name;
		}

		void packet_c::poke_name_attrib(const std::string &name_attrib)
		{
		  this->name_attrib = name_attrib;
		}

		std::string packet_c::peek_name_attrib()
		{
		  return name_attrib;
		}

		std::shared_ptr<packet_c> packet_c::new_packet(const std::string &name)
		{
		  std::shared_ptr<packet_c> packet = std::make_shared<packet_c>(environment, name);
		  packets.push_back(packet);

										// keep a list of packets by name for efficiency
										// only keeps the first packet so named
										// watchfrog #155
		  if (packets_by_name.find(name) == packets_by_name.end())
		  {
			packets_by_name.emplace(name, packet);
		  }

//C# TO C++ CONVERTER TODO TASK: A 'delete packet' statement was not added since packet was passed to a method or constructor. Handle memory management manually.
		  return packet;
		}

		void packet_c::add_packet(std::shared_ptr<packet_c> packet)
		{
		  packets.push_back(packet);

										// keep a list of packets by name for efficiency
										// only keeps the first packet so named
										// watchfrog #155
		  if (packets_by_name.find(packet->name) == packets_by_name.end())
		  {
			packets_by_name.emplace(packet->name, packet);
		  }
		}

		std::string packet_c::packet_names_csv()
		{
		  std::string s;
		  for (auto packet : packets)
		  {
			if (s.length() > 0)
			{
			  s += ", ";
			}
			s += packet->name;
		  } 
          return s;
		} // end method

		void packet_c::add_literal(const std::string &value)
		{
		  this->literal.append(value);
		}

		void packet_c::add_cdata(const std::string &value)
		{
		  this->cdatas.push_back(value);
		}

		std::shared_ptr<packet_c> packet_c::add_cdata(const std::string &field_name, const std::string &value)
		{
		  std::string F = enter("add_cdata");

		  std::shared_ptr<packet_c> cdata_packet = new_packet(field_name);
		  cdata_packet->add_attrib("type", "cdata");
		  cdata_packet->add_cdata(value);

//C# TO C++ CONVERTER TODO TASK: A 'delete cdata_packet' statement was not added since cdata_packet was used in a 'return' or 'throw' statement.
		  return cdata_packet;

		} // end method

		void packet_c::add_scalar(const std::string &name, const std::string &value)
		{
		  this->scalars[name] = value;
		}


		void packet_c::add_nvp_scalar(const std::string &key, const std::string &value)
		{
		  std::shared_ptr<packet_c> nvp_packet = new_packet("nvp");
		  nvp_packet->add_name_attrib("name", key);
		  nvp_packet->add_scalar("value", value);
		}

		void packet_c::add_attrib(const std::string &name, const std::string &value)
		{
		  this->attribs[name] = value;
		}

		void packet_c::add_name_attrib(const std::string &name, const std::string &value)
		{
		  add_attrib(name, value);
		  poke_name_attrib(name);
		}

		std::shared_ptr<packet_c> packet_c::search(const std::string &name)
		{
		  for (auto packet : packets)
		  {
			if (packet->name == name)
			{
			  return packet;
			}
		  }
		  return NULL;
		}

		std::shared_ptr<packet_c> packet_c::search_by_name_attrib(const std::string &name_attrib, const std::string &needle)
		{
		  if (name_attrib == "")
		  {
			return search(needle);
		  }
		  else
		  {
			return search_by_attrib(name_attrib, needle);
		  }
		}

		std::shared_ptr<packet_c> packet_c::search_by_attrib(const std::string &attrib, const std::string &value)
		{
		  for (auto packet : packets)
		  {
			if (packet->attribs.find(attrib) != packet->attribs.end() && packet->attribs[attrib] == value)
			{
			  return packet;
			}
		  }
		  return nullptr;
		}

		bool packet_c::has_packet(const std::string &name)
		{
		  return search(name) != nullptr;
		}

		std::shared_ptr<packet_c> packet_c::require(const std::string &name)
		{
		  std::shared_ptr<packet_c> found = search(name);

		  if (found != nullptr)
		  {
			return found;
		  }

										// when requiring a sub-packet, also recognize an empty scalar
										// watchfrog #471 
		  if (scalars.find(name) != scalars.end() && scalars[name] == "")
		  {
			scalars.erase(name);
			return new_packet(name);
		  }

		  throw_runtime_error("the packet named \"%s\" does not contain a packet named \"%s\"", this->name.c_str(), name.c_str());

          return NULL;
		}

		std::shared_ptr<packet_c> packet_c::require_unique(const std::string &name)
		{
		  std::string F = enter("require_unique");

		  std::shared_ptr<packet_c> found_packet = nullptr;

		  int count = 0;
		  for (auto packet : packets)
		  {
			if (packet->name != name)
			{
			  continue;
			}
			found_packet = packet;
			count++;
		  }

          if (count == 0)
		    throw_runtime_error("the packet named \"%s\" does not contain a packet named \"%s\"", this->name.c_str(), name.c_str());

          if (count != 1)
		    throw_runtime_error("the packet named \"%s\" contains %d packets named \"%s\"", this->name.c_str(), count, name.c_str());

		  return found_packet;
		} // end method
        
		// ----------------------------------------------------------------
		std::vector<std::string> packet_c::get_scalar_names()
		{
			std::vector<std::string> scalar_names;

			for (auto scalar : scalars)
			{
				scalar_names.push_back(scalar.first);
			}

			return scalar_names;
		} // end method

		// ------------------------------------------------------------------
		std::string packet_c::peek_scalar(const std::string &variable_name, const char *default_value)
        {
            int exists;

            return peek_scalar(variable_name, &exists, default_value);
		} // end method

		// ------------------------------------------------------------------
		std::string packet_c::peek_scalar(const std::string &variable_name, int *exists, const char *default_value)
        {
            if (scalars.find(variable_name) != scalars.end())
            {
                *exists = 1;
                return scalars[variable_name];
            }

            const char *p = variable_name.c_str();
            const char *e = std::strchr(p, '.');

            if (e == NULL)
            {
                *exists = 0;
                return std::string(default_value == NULL? "": default_value);
            }
                
            std::shared_ptr<packet_c> packet = search(std::string(p, e-p));

            if (packet == NULL)
            {
                *exists = 0;
                return std::string(default_value == NULL? "": default_value);
            }

            return packet->peek_scalar(std::string(e+1), exists, default_value);
        }
        // ----------------------------------------------------------------
		std::unordered_map<std::string, std::string> &packet_c::peek_scalars() 
		{
			return scalars;
		}

        // ----------------------------------------------------------------
        void packet_c::poke_scalar(const std::string &variable_name, std::string value)
        {
            if (scalars.find(variable_name) != scalars.end())
            {
                scalars[variable_name] = value;
            }

            const char *p = variable_name.c_str();
            const char *e = std::strchr(p, '.');

            // No more dots in variable name?
            if (e == NULL)
            {
                scalars[variable_name] = value;
                return;
            }
                
            std::string packet_name(p, e-p);
            std::shared_ptr<packet_c> packet = search(packet_name);

            if (packet == NULL)
            {
                packet = new_packet(packet_name);
            }

            packet->poke_scalar(std::string(e+1), value);
        } // end method
        // ----------------------------------------------------------------
        void packet_c::poke_scalar(const std::string &variable_name, int value)
        {
            poke_scalar(variable_name, std::to_string((long long int)value));
        } // end method
        // ----------------------------------------------------------------
        void packet_c::poke_scalar(const std::string &variable_name, double value)
        {
            poke_scalar(variable_name, std::to_string(value));
        } // end method

		// ------------------------------------------------------------------

		void packet_c::compose_html_line(std::vector<std::string> &lines, int nest_level, const std::string &line)
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

		void packet_c::compose_html_lines(std::vector<std::string> &lines, int nest_level, std::string first)
		{
		  compose_html_line(lines, nest_level, "<tr>");
		  compose_html_line(lines, nest_level + 1, "<td class=\"label packet_name" + first + "\"><div>" + escape_html(name) + "</div></td>");
		  compose_html_line(lines, nest_level + 1, "<td class=\"value" + first + "\">");
		  compose_html_line(lines, nest_level + 2, "<table cellspacing=\"0\" cellpadding=\"0\" class=\"dtack_packet\">");

		  first = " first";
		  for (auto attrib : attribs)
		  {
			compose_html_line(lines, nest_level + 2, "<tr>");
			compose_html_line(lines, nest_level + 3, "<td class=\"label" + first + "\"><div>" + escape_html(attrib.first) + "</div></td>");
			compose_html_line(lines, nest_level + 3, "<td class=\"value" + first + "\"><div>" + escape_html(attribs[attrib.first]) + "</div></td>");
			compose_html_line(lines, nest_level + 2, "</tr>");
			first = "";
		  }

		  for (auto scalar : scalars)
		  {
			compose_html_line(lines, nest_level + 2, "<tr>");
			compose_html_line(lines, nest_level + 3, "<td class=\"label" + first + "\"><div>" + escape_html(scalar.first) + "</div></td>");
			compose_html_line(lines, nest_level + 3, "<td class=\"value" + first + "\"><div>" + escape_html(scalars[scalar.first]) + "</div></td>");
			compose_html_line(lines, nest_level + 2, "</tr>");
			first = "";
		  }

		  for (auto packet : packets)
		  {
			packet->compose_html_lines(lines, nest_level + 2, first);
			first = "";
		  }

		  compose_html_line(lines, nest_level + 2, "</table>");
		  compose_html_line(lines, nest_level + 1, "</div></td>");
		  compose_html_line(lines, nest_level, "</tr>");
		}

		std::string packet_c::compose_html_string()
		{
		  std::vector<std::string> lines;

		  int nest_level = -10000000;

		  if (base_c::value(options, "indent", "yes") == "yes")
		  {
			nest_level = 0;
		  }

		  compose_html_line(lines, nest_level, "<table cellspacing=\"0\" cellpadding=\"0\" class=\"dtack_packet\">");
		  compose_html_lines(lines, nest_level + 1, std::string(" first"));
		  compose_html_line(lines, nest_level, "</table>");

		  std::string s;
		  for (auto line : lines)
		  {
			s += line;
			s += "\n";
		  }

          return s;
		} // end method

	} // end namespace
    
} // end namespace
