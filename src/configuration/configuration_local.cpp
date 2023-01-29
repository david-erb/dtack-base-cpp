#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <streambuf>

#include <chrono>
#include <thread>

#include <getopt.h>
#include <vector>

#include <unordered_map>


#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.configuration.h"

#include "dtack.base.string_token.h"
#include "dtack.base.string_walker.h"
#include "dtack.base.packet.h"

#include "configuration_local.h"

namespace dtack
{
    namespace base
    {
        // ----------------------------------------------------------------
        configuration_local_c::configuration_local_c(environment_c *environment):
          base_c(environment)
        {
            packet = std::make_shared<packet_c>(environment, "root");
        } // end method

        // ----------------------------------------------------------------
        configuration_local_c::~configuration_local_c()
        {
            // printf(".......... ~configuration_local_c %s\n", packet->name.c_str());
        } // end method

        // ----------------------------------------------------------------
        void configuration_local_c::configure(int argc, char **argv)
        {
            int c;

            int bad_args = 0;
            std::string config;

            while (1)
            {
                static struct option long_options[] =
                {
                    {"config",  required_argument, 0, 'c'},
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
                    case 'c':
                        config = optarg;
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
                throw_runtime_error("usage: --config=[json filename or json string] --log_level={DEBUG, INFO}\n");
            }



            if (config != "")
            {
                FILE *file = fopen(config.c_str(), "r");
                if (file)
                {
                    fclose(file);
                    std::ifstream stream(config.c_str());
                    std::stringstream buffer;
                    buffer << stream.rdbuf();                    
                    try
                    {
                        // Parse the file's content.
                        // If the top level has a log_level scalar, it will override the one in argv.
                        load_from_json_string(buffer.str());
                    }
                    catch(std::exception &exception)
                    {
                        throw_runtime_error("failed to parse --config json file: %s", exception.what());
                    }
                }
                else
                {
                    try
                    {
                        packet->load_from_json_string(config);
                    }
                    catch(std::exception &exception)
                    {
                        throw_runtime_error("failed to parse --config json string: %s", exception.what());
                    }
                }

                std::string json_string;
                packet->compose_jso_string(json_string);
                // printf("%s: json echo:\n%s\n", __func__, json_string.c_str());
            }

        } // end method
        
        // ----------------------------------------------------------------
        void configuration_local_c::load_from_json_string(std::string json_string)
        {
            if (packet != NULL)
            {
                // Delete the packet if we are the owner of it.
                // delete packet;
            }

            packet = std::make_shared<packet_c>(environment, "root");

            packet->load_from_json_string(json_string);

            // Automatically look for a log level in the configuration.
            int exists;
            std::string log_level_label = packet->peek_scalar("log_level", &exists, "");

            if (exists)
            {
                log_level = diwec_log_producer_c::parse_log_level_label(log_level_label);

                if (log_level == 0)
                {
                    throw_runtime_error("configuration json has invalid log level \"%s\"", log_level_label.c_str());
                
                }
            }
             
        } // end method
        
        // ----------------------------------------------------------------
        configuration_interface *configuration_local_c::section(const char *name, int *was_found)
        {
            configuration_local_c *that = new configuration_local_c(environment);

            // Let new section configuration inherit the log level assigned to the parent.
            that->log_level = this->log_level;

            that->packet = packet->search(std::string(name));

            if (that->packet != NULL)
            {
                if (was_found != NULL)
                    *was_found = 1;
            }
            else
            {
                that->packet = std::make_shared<packet_c>(environment, name);
                if (was_found != NULL)
                    *was_found = 0;
            }

            return that;
        } // end method
        
        // ----------------------------------------------------------------
        std::string configuration_local_c::peek(const char *name, std::string default_value)
        {
            std::string value = packet->peek_scalar(std::string(name), default_value.c_str());
            return value;
        } // end method

        // ----------------------------------------------------------------
        int configuration_local_c::peek(const char *name, int default_value)
        {
            std::string value = packet->peek_scalar(std::string(name), std::to_string(default_value).c_str());
            return atoi(value.c_str());
        } // end method
        // ----------------------------------------------------------------
        double configuration_local_c::peek(const char *name, double default_value)
        {
            std::string value = packet->peek_scalar(std::string(name), std::to_string(default_value).c_str());
            return atof(value.c_str());
        } // end method

        // ----------------------------------------------------------------
        void configuration_local_c::poke(const char *name, std::string value)
        {
            packet->poke_scalar(std::string(name), value);
        } // end method
        // ----------------------------------------------------------------
        void configuration_local_c::poke(const char *name, int value)
        {
            packet->poke_scalar(std::string(name), value);
        } // end method
        // ----------------------------------------------------------------
        void configuration_local_c::poke(const char *name, double value)
        {
            packet->poke_scalar(std::string(name), value);
        } // end method

        // ----------------------------------------------------------------
        int configuration_local_c::peek_log_level()
        {
            return log_level;
        } // end method
        
        // ----------------------------------------------------------------
        std::string configuration_local_c::to_json_string(bool should_indent)
        {
            std::string dump;
            if (should_indent)
                packet->set_attribute("indent", "yes");
            packet->compose_jso_string(dump);

            return dump.substr(dump.find("{"));
        } // end method

    } // end namespace

} // end namespace
