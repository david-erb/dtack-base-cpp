#pragma once

#include <vector>

namespace dtack
{
    namespace base
    {
        class Limn
        {
            public:
                virtual ~Limn() = 0;
                virtual void load_from_cli(int argc, char **argv) = 0;
                virtual void load_from_json_string(std::string json_string) = 0;
                virtual void load_from_json_file(std::string json_filename) = 0;
                virtual Limn *section(const char *name, int *was_found = NULL) = 0;
                virtual int peek_log_level() = 0;
                virtual std::vector<std::string> get_scalar_names() = 0;

                virtual std::string peek(const char *name, std::string default_value) = 0;
                virtual std::string peek(std::string name, std::string default_value) = 0;
                virtual std::string peek(const char *name, int *exists, std::string default_value) = 0;
                virtual std::string peek(std::string name, int *exists, std::string default_value) = 0;

                virtual int peek(const char *name, int default_value) = 0;
                virtual int peek(std::string name, int default_value) = 0;
                virtual int peek(const char *name, int *exists, int default_value) = 0;
                virtual int peek(std::string name, int *exists, int default_value) = 0;

                virtual double peek(const char *name, double default_value) = 0;
                virtual double peek(std::string name, double default_value) = 0;
                virtual double peek(const char *name, int *exists, double default_value) = 0;
                virtual double peek(std::string name, int *exists, double default_value) = 0;

                virtual void poke(const char *name, dtack::base::Limn *limn) = 0;
                virtual void poke(std::string name, dtack::base::Limn *limn) = 0;
                virtual void poke(const char *name, std::string value) = 0;
                virtual void poke(std::string name, std::string value) = 0;
                virtual void poke(const char *name, int value) = 0;
                virtual void poke(std::string name, int value) = 0;
                virtual void poke(const char *name, double value) = 0;
                virtual void poke(std::string name, double value) = 0;
                virtual std::string to_json_string(bool should_indent = true) = 0;

        }; // end interface

        // ----------------------------------------------------------------
        Limn *new_Limn(environment_c *environment, const char *class_name);

        Limn *new_Limn(environment_c *environment, std::string class_name);

        Limn *new_Limn(environment_c *environment);

    } // end namespace

} // end namespace
