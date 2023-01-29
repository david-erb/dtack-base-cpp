#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>

// Use the nice json library from https://github.com/nlohmann/json
#include "json.hpp"
using json_library = nlohmann::json;

#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.configuration.h"
#include "configuration_nlohmann.h"

#include "dtack.base.string_token.h"
#include "dtack.base.string_walker.h"
#include "dtack.base.packet.h"

#include "configuration_local.h"

namespace dtack
{
    namespace base
    {
        configuration_factory_c::configuration_factory_c(environment_c *environment):
            base_c(environment)
        {

        }

        // --------------------------------------------------------------------
        // Manufacture a concrete implementation, but return only a reference to the interface.
        configuration_interface *configuration_factory_c::manufacture(const char *class_name)
        {
            configuration_interface *instance = NULL;

            if (class_name == NULL)
            {
                instance = new configuration_local_c(environment);
            }
            else
            if (!strcmp(class_name, "configuration_nlohmann_c"))
            {
                instance = new configuration_nlohmann_c(environment);
            }
            else
            if (!strcmp(class_name, "configuration_local_c"))
            {
                instance = new configuration_local_c(environment);
            }

            if (instance == NULL)
                throw std::runtime_error("unknown configuration implementation class name \"" + std::string(class_name) + "\"");

            return instance;
        }

    } // end namespace

} // end namespace
