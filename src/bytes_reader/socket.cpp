#include <chrono>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include <unordered_map>

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.limn.h>

#include "dtack.base.bytes_reader.h"
#include "bytes_reader/base.h"
#include "bytes_reader/socket.h"

namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {
            
            // ----------------------------------------------------------------
            socket_c::socket_c(dtack::base::environment_c *environment):
                base_c(environment)
            {
            }

            // ----------------------------------------------------------------
            socket_c::~socket_c()
            {
            }

            // ----------------------------------------------------------------
            void socket_c::open()
            {
            }

            // ----------------------------------------------------------------
            void socket_c::read(char *data, size_t size)
            {
            }

            // ----------------------------------------------------------------
            void socket_c::close()
            {
            }

        } // end namespace

    } // end namespace

} // end namespace