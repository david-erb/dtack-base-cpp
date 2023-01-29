namespace dtack
{
    namespace base
    {
        class configuration_nlohmann_c: public base_c, public configuration_interface
        {
            public:
                configuration_nlohmann_c(environment_c *environment);
                ~configuration_nlohmann_c();
                virtual void configure(int argc, char **argv);
                virtual void load_from_json_string(std::string json_string);
                virtual configuration_interface *section(const char *name, int *was_found = NULL);
                virtual int peek_log_level();
                virtual std::string peek(const char *name, std::string default_value);
                virtual int peek(const char *name, int default_value);
                virtual double peek(const char *name, double default_value);
                virtual void poke(const char *name, std::string value);
                virtual void poke(const char *name, int value);
                virtual void poke(const char *name, double value);
                virtual std::string to_json_string(bool should_indent = true);

            private:
                int log_level = 0;
                json_library json;
        }; // end class
    } // end namespace
} // end namespace
