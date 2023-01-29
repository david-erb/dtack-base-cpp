#pragma once

namespace dtack
{
	namespace base
	{

        typedef double clock_seconds_t;

        std::string printf_to_string(const char *format, ...);
        std::string printf_to_string_va(const char *format, va_list args);
        void throw_runtime_error(const char *format, ...);
        void *allocate_from_heap(const char *caller, const char *purpose, int count, int size);
        clock_seconds_t get_clock_seconds();
        int mkdir_p(const char *dir, const mode_t mode);
        void write_file(const char *filename, void *data, int size);
        void read_file(const char *filename, void **data, int *size);
        
        void get_process_memory_usage(double &vm_usage, double &resident_set);

        std::string format_compiled_date(const char *compiled_date, const char *compiled_time);
        std::string extract_class_and_method_name(const std::string &prettyFunction);

        std::string escape_html(const std::string &s);
        std::string unescape_html(const std::string &s);
        std::string replace(std::string source, const std::string find, const std::string replace);

        class Version
        {
        public:
            std::string module;
            std::string version;
            std::string compile_date_string;
            std::string to_string()
            {
                return printf_to_string("%s %s compiled %s", module.c_str(), version.c_str(), compile_date_string.c_str());
            };
        };

        Version version();

        
	  class base_c
	  {
		// -----------------------------------------------------------
	        public:
                base_c(environment_c *environment);
		        virtual ~base_c();
                virtual std::string enter(std::string function_name);


                virtual std::string require(std::unordered_map<std::string, std::string> &map, std::string &key);
                virtual std::string value(std::unordered_map<std::string, std::string> &map, const std::string &key, const std::string &default_value);
                virtual std::string value(std::unordered_map<std::string, std::string> &map, const char *key, const char *default_value);

                virtual std::string get_attribute(const char *key, const char *default_value = NULL);
                virtual void set_attribute(const char *key, const char *value);

            public:
            	std::unordered_map<std::string, std::string> attributes;

            protected:
                environment_c *environment;
      };

	} // end namespace

} // end namespace
