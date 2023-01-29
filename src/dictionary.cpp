#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <dtack.base.dictionary.h>

namespace dtack
{
	namespace base
	{

		std::string dictionary_c::value(dictionary_c *dictionary, const std::string &keyword, const std::string &deefault)
		{
		  if (dictionary->empty())
		  {
			return deefault;
		  }
		  else
		  {
			return dictionary->value(keyword, deefault);
		  }
		} // end static method

		dictionary_c::dictionary_c()
		{
		}

		void dictionary_c::append(dictionary_c *dictionary)
		{
		  for (auto key : dictionary)
		  {
			if (this->find(key.first) == this->end())
			{
			  this->operator[](key.first) = dictionary[key.first];
			}
		  }
		}

		void dictionary_c::append(const std::string &value)
		{
		  this->operator[](std::to_string(this->size())) = value;
		}

		void dictionary_c::append(const std::string &name, const std::string &value)
		{
		  if (this->find(name) != this->end())
		  {
			this->operator[](name) += value;
		  }
		  else
		  {
			this->operator[](name) = value;
		  }
		}

		void dictionary_c::prepend(const std::string &name, const std::string &value)
		{
		  if (this->find(name) != this->end())
		  {
			this->operator[](name) = value + this->operator[](name);
		  }
		  else
		  {
			this->operator[](name) = value;
		  }
		}

		std::setwstring dictionary_c(const std::string &key, const std::string &value)
		{
		  this->operator[](key) = value;

		  return this->operator[](key);
		}

		std::string dictionary_c::set_if_not_set(const std::string &key, const std::string &value)
		{
		  if (this->find(key) == this->end())
		  {
			this->operator[](key) = value;
		  }

		  return this->operator[](key);
		}

		void dictionary_c::supercede_from(dictionary_c *dictionary)
		{
		  for (auto key : dictionary)
		  {
			this->operator[](key.first) = dictionary[key.first];
		  }
		}

		std::string dictionary_c::csv(const std::string &after_key, const std::string &after_pair)
		{
		  StringBuilder *s = new StringBuilder();

		  for (auto key : this)
		  {
			if (s->length() > 0)
			{
			  s->append(after_pair);
			}
			s->append(key.first);
			s->append(after_key);
			s->append(this->operator[](key.first));
		  }

		  delete s;
		  return s->toString();
		}

		std::string dictionary_c::csv_keys()
		{
		  return csv_keys(",");
		}

		std::string dictionary_c::csv_keys(const std::string &separator, csv_decorator_d decorator)
		{
		  StringBuilder *s = new StringBuilder();

		  for (auto key : this)
		  {
			if (s->length() > 0)
			{
			  s->append(separator);
			}
			if (decorator == nullptr)
			{
			  s->append(key.first);
			}
			else
			{
			  s->append(decorator(key.first));
			}
		  }

		  delete s;
		  return s->toString();
		}

		std::string dictionary_c::csv_values()
		{
		  return implode(",");
		}

		std::string dictionary_c::csv_values(const std::string &separator, csv_decorator_d decorator)
		{
		  return implode(separator, decorator);
		}

		std::string dictionary_c::implode(const std::string &separator, csv_decorator_d decorator)
		{
		  StringBuilder *s = new StringBuilder();

		  for (auto value : this)
		  {
			if (s->length() > 0)
			{
			  s->append(separator);
			}
			if (decorator == nullptr)
			{
			  s->append(value.second);
			}
			else
			{
			  s->append(decorator(value.second));
			}
		  }

		  delete s;
		  return s->toString();
		}

		std::string dictionary_c::compose_start(const std::string &what)
		{
		  std::string s = "";
		  s += "<table cellspacing=0 cellpadding=0 class='tohtml_table'><!-- compose start -->\n";
		  s += "<tr>\n";
		  if (what != "")
		  {
			s += "<td class='tohtml_td'>" + what + "</td>\n";
		  }
		  s += "<td class='tohtml_td'>\n";

		  return s;
		}

		std::string dictionary_c::compose_finish()
		{
		  std::string s = "";

		  s += "</td>\n";
		  s += "</tr>\n";
		  s += "</table><!-- compose finish -->\n";
		  return s;
		}

		std::string dictionary_c::compose(const std::string &what)
		{
		  return compose_html(what, false);
		}

		std::string dictionary_c::compose_html(const std::string &what)
		{
		  return compose_html(what, false);
		}

		std::string dictionary_c::compose_html(const std::string &what, bool htmlescape)
		{
		  StringBuilder *s = new StringBuilder();

		  compose_html(what, htmlescape, s);

//C# TO C++ CONVERTER TODO TASK: A 'delete s' statement was not added since s was passed to a method or constructor. Handle memory management manually.
		  return s->toString();
		}

		void dictionary_c::compose_html(const std::string &what, bool htmlescape, StringBuilder *s)
		{
		  s->append(compose_start(what));

		  if (this->size() > 0)
		  {
			for (auto key : this)
			{
			  if (htmlescape)
			  {
				s->append(key.first + " = " + dtack::u::htmlescape(this->operator[](key.first) + ""));
			  }
			  else
			  {
				s->append(key.first + " = " + this->operator[](key.first));
			  }

			  s->append("<br>\n");
			}
		  }
		  else
		  {
			s->append("dictionary is empty");
		  }

		  s->append(this->compose_finish());

		}

		std::string dictionary_c::compose_text(const std::string &what)
		{
		  StringBuilder *s = new StringBuilder(what + ":\n");

		  if (this->size() > 0)
		  {
			for (auto key : this)
			{
			  s->append(key.first + " = " + this->operator[](key.first));

			  s->append("\n");
			}
		  }
		  else
		  {
			s->append("dictionary is empty");
		  }

		  delete s;
		  return s->toString();
		}

		std::string dictionary_c::compose_xml(const std::string &what)
		{
		  StringBuilder *s = new StringBuilder(what + ":\n");

		  if (this->size() > 0)
		  {
			for (auto key : this)
			{
			  s->append(key.first + " = " + this->operator[](key.first));

			  s->append("\n");
			}
		  }
		  else
		  {
			s->append("dictionary is empty");
		  }

		  dtack::u::xmlescape(s);

//C# TO C++ CONVERTER TODO TASK: A 'delete s' statement was not added since s was passed to a method or constructor. Handle memory management manually.
		  return s->toString();
		}

		std::string dictionary_c::compose_cgi(bool should_escape)
		{
		  StringBuilder *s = new StringBuilder("");

		  if (this->size() > 0)
		  {
			for (auto key : this)
			{
			  if (s->length() > 0)
			  {
				s->append("&");
			  }

			  if (should_escape)
			  {
				s->append(dtack::u::urlescape(key.first) + "=" + dtack::u::urlescape(this->operator[](key.first)));
			  }
			  else
			  {
				s->append(key.first + "=" + this->operator[](key.first));
			  }
			}
		  }

		  delete s;
		  return s->toString();
		} // end method

		std::string dictionary_c::compose_cgi_fully()
		{
		  StringBuilder *s = new StringBuilder("");

		  if (this->size() > 0)
		  {
			for (auto key : this)
			{
			  if (s->length() > 0)
			  {
				s->append("&");
			  }

			  s->append(dtack::u::urlescape(key.first));
			  s->append("=");
			  StringBuilder *value = new StringBuilder(this->operator[](key.first));
			  s->append(dtack::u::urlescape_fully(value));

//C# TO C++ CONVERTER TODO TASK: A 'delete value' statement was not added since value was passed to a method or constructor. Handle memory management manually.
			}
		  }

		  delete s;
		  return s->toString();
		} // end method

		StringBuilder *dictionary_c::compose_json()
		{
		  return compose_json("");
		} // end method

		StringBuilder *dictionary_c::compose_json(const std::string &line_prefix)
		{
		  StringBuilder *s = new StringBuilder("");

		  if (this->size() > 0)
		  {
			for (auto key : this)
			{
			  if (s->length() > 0)
			  {
				s->append(",\n");
			  }

			  std::string key_escaped = dtack::u::jsoescape(key.first);
			  if (key_escaped != key.first)
			  {
				key_escaped = "\"" + key_escaped + "\"";
			  }
			  s->append(line_prefix + key_escaped + ": \"" + dtack::u::jsoescape(this->operator[](key.first)) + "\"");
			}
		  }

//C# TO C++ CONVERTER TODO TASK: A 'delete s' statement was not added since s was used in a 'return' or 'throw' statement.
		  return s;
		}

		dictionary_c *dictionary_c::copy()
		{
		   dictionary_c *dictionary = new dictionary_c(name);
		   for (auto key : this)
		   {
			 dictionary[key.first] = this->operator[](key.first);
		   }

//C# TO C++ CONVERTER TODO TASK: A 'delete dictionary' statement was not added since dictionary was used in a 'return' or 'throw' statement.
		   return dictionary;
		}

		std::string dictionary_c::require(const std::string &key)
		{
		  if (this->find(key) == this->end())
		  {
			  throw std::runtime_error("the key " + dtack::u::vts(key) + " is not defined in the dictionary" + (this->name != ""? " named " + dtack::u::vts(this->name): ""));
		  }
			return System::Collections::Generic::Dictionary<string, string>::operator[](key);
		}

		std::string dictionary_c::value(const std::string &key, const std::string &deefault)
		{
		  if (this->find(key) == this->end())
		  {
			return deefault;
		  }
		  else
		  {
			return System::Collections::Generic::Dictionary<string, string>::operator[](key);
		  }
		} // end method

		std::string dictionary_c::vts(const std::string &key)
		{
		  if (this->find(key) == this->end())
		  {
			return "*unset*";
		  }
		  else
		  {
			return dtack::u::vts(System::Collections::Generic::Dictionary<string, string>::operator[](key));
		  }
		} // end method

		void dictionary_c::write_ini(const std::string &filename)
		{
		  // const string F = "write_ini";

											/* create the directory hierarchy */
		  FileSystem::createDirectory(FileSystem::getDirectoryName(filename));

											/* open the output file */
		  StreamWriter *stream = File::CreateText(filename);

											/* write the section header */
			stream->Write("[" + name + "]\n");
			for (auto column : this)
			{
			stream->Write(column.first + " = " + StringHelper::replace(StringHelper::replace(this->operator[](column.first), "\r", ""), "\n", "\\n") + "\n");
			}

		  stream->Close();
		} // end method

		void dictionary_c::translate_regex(dictionary_c *translation)
		{
		  // static string F = "translate_regex";

		  try
		  {
			std::vector<std::string> keys(this->getKeys()->Count);
			this->getKeys()->CopyTo(keys, 0);
			for (auto column : keys)
			{
			  for (auto from : translation)
			  {
				this->operator[](column) = Regex::Replace(this->operator[](column), from.first, translation[from.first]);
			  }
			}
		  }
		  catch (const std::runtime_error &e)
		  {
			throw std::runtime_error("could not translate dictionary: " + e.what());
		  }
		} // end method

		void dictionary_c::translate(dictionary_c *translation)
		{
		  //const string F = "translate";

		  try
		  {
			std::vector<std::string> keys(this->getKeys()->Count);
			this->getKeys()->CopyTo(keys, 0);
			for (auto column : keys)
			{
			  if (translation->find(this->operator[](column)) != translation->end())
			  {
				//track(this, F, column + " translates from `" + this[column] + "' to `" + translation[this[column]] + "'");
				this->operator[](column) = translation[this->operator[](column)];
			  }
			}
		  }
		  catch (const std::runtime_error &e)
		  {
			throw std::runtime_error("could not translate dictionary: " + e.what());
		  }
		} // end method

		void dictionary_c::fill_from_cgi_string(const std::string &cgi_string)
		{
		  fill_from_cgi_string(cgi_string, false);
		} // end method

		void dictionary_c::fill_from_cgi_string(const std::string &cgi_string, bool should_unescape)
		{
		  Regex *keyword_pattern = new Regex("^([^#;'*=]+)[=](.*)$");

		  std::vector<std::string> cgis = StringHelper::split(cgi_string, L'&');
		  for (auto cgi : cgis)
		  {
			std::string line = cgi.Trim();
													/* see if this is a keyword = value line */
			Match *keyword_match = keyword_pattern->Match(line);

			if (keyword_match->Success)
			{
											/* get the keyword */
			  std::string keyword = keyword_match->Groups[1]->Captures[0]->Value;
			  std::string value = StringHelper::trim(keyword_match->Groups[2]->Captures[0]->Value);

			  if (should_unescape)
			  {
				StringBuilder tempVar(keyword);
				keyword = dtack::u::urlunescape_fully(&tempVar).toString();
				StringBuilder tempVar2(value);
				value = dtack::u::urlunescape_fully(&tempVar2).toString();
			  }

											/* add it to the current dictionary we are building */
			  this->operator[](StringHelper::trim(keyword)) = value;
			}
		  }

			delete keyword_pattern;
		} // end method

		void dictionary_c::plus(const std::string &field, const std::string &value)
		{
			double d = dtack::u::s2d(value);
			if (this->find(field) != this->end())
			{
			  d += dtack::u::s2d(this->operator[](field));
			}

			this->operator[](field) = std::to_string(d);
		}

		void dictionary_c::max(const std::string &field, const std::string &value)
		{
			double d = dtack::u::s2d(value);
			if (this->find(field) != this->end())
			{
			  d = std::max(d, dtack::u::s2d(this->operator[](field)));
			}

			this->operator[](field) = std::to_string(d);
		}

		void dictionary_c::min(const std::string &field, const std::string &value)
		{
			double d = dtack::u::s2d(value);
			if (this->find(field) != this->end())
			{
			  d = std::min(d, dtack::u::s2d(this->operator[](field)));
			}

			this->operator[](field) = std::to_string(d);
		}
	}
}
