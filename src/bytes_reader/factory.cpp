#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <unistd.h>


#include <unordered_map>

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.limn.h>

#include "dtack.base.bytes_reader.h"
#include "bytes_reader/base.h"
#include "bytes_reader/socket.h"
#include "bytes_reader/binary_file.h"


namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {

            factory_c::factory_c(dtack::base::environment_c *environment):
                base_c(environment)
            {

            }

            // ------------------------------------------------------------------------
            // Manufacture a concrete implementation, but return only a reference to the interface.
            interface_i *factory_c::manufacture(std::string type)
            {
                return manufacture(type.c_str());
            }

            // ------------------------------------------------------------------------
            // Manufacture a concrete implementation, but return only a reference to the interface.
            interface_i *factory_c::manufacture(const char *type)
            {
                interface_i *instance = NULL;

                if (!strcmp(type, "socket"))
                {
                    instance = new socket_c(environment);
                }
                else
                if (!strcmp(type, "binary_file"))
                {
                    instance = new binary_file_c(environment);
                }

                if (instance == NULL)
                    throw_runtime_error("unknown bytes reader \"%s\"", type);

                return instance;
            }

        } // end namespace

    } // end namespace
    
} // end namespace