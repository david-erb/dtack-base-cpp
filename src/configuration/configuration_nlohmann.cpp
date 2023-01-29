#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <fstream>
#include <chrono>
#include <thread>

#include <getopt.h>

// Use the nice json library from https://github.com/nlohmann/json
#include "json.hpp"
using json_library = nlohmann::json;

#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.configuration.h"
#include "configuration_nlohmann.h"

namespace dtack
{
    namespace base
    {
        // ----------------------------------------------------------------
        configuration_nlohmann_c::configuration_nlohmann_c(environment_c *environment):
          base_c(environment)
        {

        } // end method

        // ----------------------------------------------------------------
        configuration_nlohmann_c::~configuration_nlohmann_c()
        {
            // printf("~configuration_nlohmann_c\n");
        } // end method

        // ----------------------------------------------------------------
        void configuration_nlohmann_c::configure(int argc, char **argv)
        {

            int c;

            int bad_args = 0;
            std::string configuration;
            std::string filename;

            while (1)
            {
                static struct option long_options[] =
                {
                    {"c",  required_argument, 0, 'c'},
                    {"filename",  required_argument, 0, 'f'},
                    {"log_level",  required_argument, 0, 'v'},
                    {0, 0, 0, 0}
                };

                /* getopt_long stores the option index here. */
                int option_index = 0;

                c = getopt_long(argc, argv, "", long_options, &option_index);

                if (c == -1)
                break;

                switch (c)
                {
                    case 'f':
                        filename = optarg;
                    break;

                    case 'c':
                        configuration = optarg;
                    break;

                    case 'v':
                        log_level = diwec_log_producer_c::parse_log_level_label(std::string(optarg));

                        if (log_level == 0)
                        {
                            printf("invalid log level %s\n", optarg);
                            bad_args = 1;
                        }
                    break;

                    case '?':
                        bad_args = 1;
                    break;
                }
            }

            if (bad_args)
            {
                throw_runtime_error("usage: --c=configuration (filename or json) --filename=configuration_file --log_level={DEBUG, INFO}\n");
            }

            if (filename != "")
            {
                std::ifstream file(filename.c_str());
                file >> json;
            }

        } // end method
        
        // ----------------------------------------------------------------
        configuration_interface *configuration_nlohmann_c::section(const char *name, int *was_found)
        {
            throw_runtime_error("section method not implemented");
            return NULL;
        } // end method
        
        // ----------------------------------------------------------------
        void configuration_nlohmann_c::load_from_json_string(std::string json_string)
        {
            throw_runtime_error("section method not implemented");
        } // end method
                

        // ----------------------------------------------------------------
        std::string configuration_nlohmann_c::peek(const char *name, std::string default_value)
        {
            std::string sname = name;
            if (!json[sname].is_null())
            {
                std::string value = json[name].get<std::string>();
                return value;
            }
            else 
                return default_value;
        } // end method

        // ----------------------------------------------------------------
        int configuration_nlohmann_c::peek(const char *name, int default_value)
        {
            std::string sname = name;
            if (!json[sname].is_null())
            {
                int value = json[name].get<int>();
                return value;
            }
            else 
                return default_value;
        } // end method
        // ----------------------------------------------------------------
        double configuration_nlohmann_c::peek(const char *name, double default_value)
        {
            std::string sname = name;
            if (!json[sname].is_null())
            {
                double value = json[name].get<double>();
                return value;
            }
            else 
                return default_value;
        } // end method
        // ----------------------------------------------------------------
        void configuration_nlohmann_c::poke(const char *name, std::string value)
        {
        } // end method
        // ----------------------------------------------------------------
        void configuration_nlohmann_c::poke(const char *name, int value)
        {
        } // end method
        // ----------------------------------------------------------------
        void configuration_nlohmann_c::poke(const char *name, double value)
        {
        } // end method
        
        // ----------------------------------------------------------------
        int configuration_nlohmann_c::peek_log_level()
        {
            return log_level;
        } // end method
        
        // ----------------------------------------------------------------
        std::string configuration_nlohmann_c::to_json_string(bool should_indent)
        {
            return json.dump(4);
        } // end method

    } // end namespace

} // end namespace
