#pragma once

namespace dtack
{
    namespace base
    {
        class configuration_interface
        {
            public:
                virtual ~configuration_interface() = 0;
                virtual void configure(int argc, char **argv) = 0;
                virtual void load_from_json_string(std::string json_string) = 0;
                virtual configuration_interface *section(const char *name, int *was_found = NULL) = 0;
                virtual int peek_log_level() = 0;
                virtual std::string peek(const char *name, std::string default_value) = 0;
                virtual int peek(const char *name, int default_value) = 0;
                virtual double peek(const char *name, double default_value) = 0;
                virtual void poke(const char *name, std::string value) = 0;
                virtual void poke(const char *name, int value) = 0;
                virtual void poke(const char *name, double value) = 0;
                virtual std::string to_json_string(bool should_indent = true) = 0;

        }; // end class

        class configuration_factory_c: public base_c
        {
            public:
                configuration_factory_c(environment_c *environment);

            public: 
                virtual configuration_interface *manufacture(const char *class_name = NULL);
        }; // end class

    } // end namespace

} // end namespace
