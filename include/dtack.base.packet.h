#pragma once

namespace dtack
{
	namespace base
	{
	  // ----------------------------------------------------------------------------
	  // --------------------------------------------------------------------
	  class packet_c : virtual public base_c
	  {

		// ------------------------------------------------------------------
		public:
		packet_c(environment_c *environment);

		// ------------------------------------------------------------------
		packet_c(environment_c *environment, const std::string &name);

		virtual ~packet_c();

		std::shared_ptr<packet_c> clone(environment_c *environment);



		bool should_compose_text_with_full_tag_closure = true;


	private:
		// ------------------------------------------------------------------
		std::string compose_text_value(const std::string &value);

		// ------------------------------------------------------------------
		std::string compose_text_scalar(const std::string &scalar_name);

		// ------------------------------------------------------------------
		void compose_text_line(std::vector<std::string> &lines, int nest_level, const std::string &line);

		// ------------------------------------------------------------------

		void compose_text_lines(std::vector<std::string> &lines, int nest_level);

    public:
		// ------------------------------------------------------------------
		void compose_text_string(std::string &s);

		// -----------------------------------------------------------

		void sync(std::shared_ptr<packet_c> that);

		void supercede_from(std::shared_ptr<packet_c> that);


		// -----------------------------------------------------------

	  private:
		  std::string indent = "                                                            ";

	  private:
		  std::string name_attrib = "";

      public:
			// ------------------------------------------------------------------
			std::string peek_name();

			// ------------------------------------------------------------------
			void poke_name_attrib(const std::string &name_attrib);

			// ------------------------------------------------------------------
			std::string peek_name_attrib();

			// ------------------------------------------------------------------
			std::shared_ptr<packet_c> new_packet(const std::string &name);

			// ------------------------------------------------------------------
			void add_packet(std::shared_ptr<packet_c> packet);

			// ------------------------------------------------------------------

			std::string packet_names_csv();

			// ------------------------------------------------------------------
			void add_literal(const std::string &value);

			// ------------------------------------------------------------------
			void add_cdata(const std::string &value);

			// --------------------------------------------------------------------

			std::shared_ptr<packet_c> add_cdata(const std::string &field_name, const std::string &value);

			// ------------------------------------------------------------------
			void add_scalar(const std::string &name, const std::string &value);
			// ------------------------------------------------------------------
			void add_nvp_scalar(const std::string &key, const std::string &value);
			// ------------------------------------------------------------------
			void add_attrib(const std::string &name, const std::string &value);
			// ------------------------------------------------------------------
			void add_name_attrib(const std::string &name, const std::string &value);
			// ------------------------------------------------------------------
			// search for packet by name
			std::shared_ptr<packet_c> search(const std::string &name);
			// ------------------------------------------------------------------
			// search for packet by name
			std::shared_ptr<packet_c> search_by_name_attrib(const std::string &name_attrib, const std::string &needle);
			// ------------------------------------------------------------------
			// search for packet having given attrib value
			std::shared_ptr<packet_c> search_by_attrib(const std::string &attrib, const std::string &value);
			// ------------------------------------------------------------------
			// search for packet by name
			bool has_packet(const std::string &name);
			// ------------------------------------------------------------------
			// search for packet by name, throw an assertion exception if it fails
			std::shared_ptr<packet_c> require(const std::string &name);
			// ------------------------------------------------------------------
            std::vector<std::string> get_scalar_names();
	    	std::string peek_scalar(const std::string &variable_name, const char *default_value = NULL);
		    std::string peek_scalar(const std::string &variable_name, int *exists, const char *default_value = NULL);
			std::unordered_map<std::string, std::string> &peek_scalars();
            void poke_scalar(const std::string &variable_name, std::string value);
            void poke_scalar(const std::string &variable_name, int value);
            void poke_scalar(const std::string &variable_name, double value);

		  // ------------------------------------------------------------------
		  // search for packet by name, throw an assertion exception if not found or more than one
		  std::shared_ptr<packet_c> require_unique(const std::string &name);

		// ------------------------------------------------------------------
	private:
		void compose_html_line(std::vector<std::string> &lines, int nest_level, const std::string &line);

		// ------------------------------------------------------------------
	public:
		void compose_html_lines(std::vector<std::string> &lines, int nest_level, std::string first);

		// ------------------------------------------------------------------
		std::string compose_html_string();



		private:
			// ------------------------------------------------------------------
			std::string compose_jso_value(const std::string &s);

			// ------------------------------------------------------------------
			void compose_jso_line(std::vector<std::string> &lines, int nest_level, const std::string &line);

			// ------------------------------------------------------------------
		public:
			void compose_jso_lines(std::vector<std::string> &lines, int nest_level);

			// ------------------------------------------------------------------
			std::string compose_jso_string();
			void compose_jso_string(std::string &s);

		// ------------------------------------------------------------------
	public:
		void load_from_json_filename(const std::string &filename);

		// ------------------------------------------------------------------
		void load_from_json_string(const std::string &s);

    private:
		// ------------------------------------------------------------------
		void load_from_json_string_walker(std::shared_ptr<string_walker_c> string_walker);


		// ------------------------------------------------------------------
		void load_from_json_string_walker(std::shared_ptr<string_walker_c> string_walker, std::shared_ptr<string_token_c> had_token);


		// ------------------------------------------------------------------
		void handle_something(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_exhausted(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_colon(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_comma(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_left_curly_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_right_curly_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_left_square_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_right_square_bracket(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_literal(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		void handle_other(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

		// ------------------------------------------------------------------
		std::string compose_array_name();

		// ------------------------------------------------------------------
		std::string compose_object_name();

		// ------------------------------------------------------------------
		std::string compose_location();

		// ------------------------------------------------------------------
		void raise_unexpected_after(std::shared_ptr<string_token_c> have_token, std::shared_ptr<string_token_c> had_token);

    public:
			// ------------------------------------------------------------------

			void legalize_names();

    public:
		  std::unordered_map<std::string, std::string> options;
		  std::string name;

    private:
		  std::unordered_map<std::string, std::string> attribs;
		  std::unordered_map<std::string, std::string> scalars;
		  std::vector<std::string> cdatas = std::vector<std::string>();
		  std::string literal = "";
		  std::vector<std::shared_ptr<packet_c>> packets = std::vector<std::shared_ptr<packet_c>>();

										   // capture the value from a packet which has attribs but contains no packets
		  std::string value = "";


		  std::unordered_map<std::string, std::shared_ptr<packet_c>> packets_by_name;

										   // whether to consider a node with attribs but no scalar or packet children to be a packet
										   // default is off to conform to legacy
										   // watchfrog #226
		  bool parse_node_with_attribs_as_packet = false;

		  bool should_track_parse_detail = false;

		  bool should_compose_linebreaks = true;

		std::shared_ptr<string_walker_c> string_walker;
		std::shared_ptr<string_token_c> property_name_token = nullptr;
		bool should_quit = false;


	  }; // end class
	}
}
