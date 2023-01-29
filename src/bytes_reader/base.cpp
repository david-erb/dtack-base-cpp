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

namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {
            // ----------------------------------------------------------------
            interface_i::~interface_i()
            {
            }


            // ----------------------------------------------------------------
            base_c::base_c(dtack::base::environment_c *environment):
                dtack::base::base_c(environment),
                limn(NULL)
            {
            }

            // ----------------------------------------------------------------
            base_c::~base_c()
            {
            }

            // ----------------------------------------------------------------
            void base_c::configure(dtack::base::Limn *limn)
            {
                this->limn = limn;
            }

            // ----------------------------------------------------------------
            void base_c::open()
            {
            }

            // ----------------------------------------------------------------
            void base_c::read(char *data, int size)
            {
            }

            // ----------------------------------------------------------------
            void base_c::close()
            {
            }

        } // end namespace

    } // end namespace

} // end namespace
