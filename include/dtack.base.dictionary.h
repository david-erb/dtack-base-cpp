#pragma once

namespace dtack
{
	namespace base
	{
	  // ----------------------------------------------------------------------------
	  // this class is a dictionary of strings
	  // each item in this named list is a string
	  // the names themselves are also strings
	  // this object has a name fieldwhich applies to itself
	  class dictionary_c : public std::unordered_map<std::string, std::string>
	  {

		// ---------------------------------------------------------------------------
	public:
		virtual double convert(const std::string &name, double value);

		// ---------------------------------------------------------------------------
		virtual long long convert(const std::string &name, long long value);

		// ---------------------------------------------------------------------------
		virtual int convert(const std::string &name, int value);

		// ---------------------------------------------------------------------------
		virtual bool convert(const std::string &name, bool value);



		// ---------------------------------------------------------------
		static bool is_affirmative(dictionary_c *dictionary, const std::string &keyword);



		// ----------------------------------------------------------------------------

		void split_and_trim_into_keys(const std::string &source);



		static std::string value(dictionary_c *dictionary, const std::string &keyword, const std::string &deefault);

		// ----------------------------------------------------------------------------
		//public string default_value = null;
		//public override string this[string key] { get { if (default_value && !ContainsKey(key)) return default_value; return base[key] ;} }

		std::string name = "";
		  std::string sort_field = "";

		// ----------------------------------------------------------------------------
		dictionary_c();

		// ----------------------------------------------------------------------------
		// constructor, given the object's name
		// also given an array of strings, even elements are names, odd elements are values
		dictionary_c(const std::string &name, std::vector<std::string> &values);

		// -----------------------------------------------------------
		// replace value within all keywords of the dictionary
		// watchfrog #377

		dictionary_c *replace(const std::string &this_value, const std::string &that_value);


		// ----------------------------------------------------------------------------
		// add values from another dictionary which don't already exist in this one
		void append(dictionary_c *dictionary);


		// ----------------------------------------------------------------------------
		// append value with next numeric key

		void append(const std::string &value);


		// ----------------------------------------------------------------------------
		// accept and store a string value for replacement during action.variable_c

		void append(const std::string &name, const std::string &value);

		// ----------------------------------------------------------------------------
		//

		void prepend(const std::string &name, const std::string &value);

		// ----------------------------------------------------------------------------

		std::string set(const std::string &key, const std::string &value);

		// ----------------------------------------------------------------------------

		std::string set_if_not_set(const std::string &key, const std::string &value);

		// ----------------------------------------------------------------------------
		// add values from another dictionary, overwriting those which already exist in this one
		void supercede_from(dictionary_c *dictionary);
		// -----------------------------------------------------------
		// return csv of keys in this dictionary
		std::string csv(const std::string &after_key, const std::string &after_pair);

		// -----------------------------------------------------------
		// return csv of keys in this dictionary
		std::string csv_keys();

		// -----------------------------------------------------------
		// return csv of values in this dictionary
		std::string csv_values();

		// ----------------------------------------------------------------
		// start a "tohtml" HTML container
	private:
		std::string compose_start(const std::string &what);
	  // ----------------------------------------------------------------
	  // finish a "tohtml" HTML container
	  std::string compose_finish();

	  // -----------------------------------------------------------
	  // return html describing this variable
  public:
	  std::string compose(const std::string &what);

	  // -----------------------------------------------------------
	  // return html describing this variable
	  std::string compose_html(const std::string &what);

	  // -----------------------------------------------------------
	  // return html describing this variable
	  std::string compose_html(const std::string &what, bool htmlescape);

	  // -----------------------------------------------------------
	  // return html describing this variable
	  std::string compose_text(const std::string &what);

	  // -----------------------------------------------------------
	  // return html describing this variable
	  std::string compose_xml(const std::string &what);

	  // -----------------------------------------------------------
	  // return cgi describing this variable
	  std::string compose_cgi(bool should_escape = true);

	  // -----------------------------------------------------------
	  // return cgi describing this variable
	  std::string compose_cgi_fully();

	  // -----------------------------------------------------------
	  // return copy of this dictionary
	  dictionary_c *copy();


		// -------------------------------------------------------
		std::string require(const std::string &key);

		// -------------------------------------------------------
		std::string value(const std::string &key, const std::string &deefault);

		// -------------------------------------------------------
		std::string vts(const std::string &key);

		// ------------------------------------------------------------------
		void write_ini(const std::string &filename);

		// -------------------------------------------------------
		// translate all values in the dictionary
		// translation keys provide the regex pattern and values provide the replacement
		// every value in the dictionary undergoes every pattern/replacement operation
		virtual void translate_regex(dictionary_c *translation);


		// -------------------------------------------------------
		// translate all values in the dictionary
		// translation keys provide the regex pattern and values provide the replacement
		// every value in the dictionary undergoes every pattern/replacement operation
		virtual void translate(dictionary_c *translation);

		// ----------------------------------------------------------------------------
		// fill the dictionary from a string containing standard cgi format
		void fill_from_cgi_string(const std::string &cgi_string);

		// ----------------------------------------------------------------------------
		// fill the dictionary from a string containing standard cgi format
		void fill_from_cgi_string(const std::string &cgi_string, bool should_unescape);
		// ----------------------------------------------------------------------
		void plus(const std::string &field, const std::string &value);

		// ----------------------------------------------------------------------
		void max(const std::string &field, const std::string &value);
		// ----------------------------------------------------------------------
		void min(const std::string &field, const std::string &value);

	  };
	}
}
