#include <system_error>
#include <stdexcept>
#include <chrono>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <unordered_map>

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.limn.h>

#include "dtack.base.bytes_reader.h"
#include "bytes_reader/base.h"
#include "bytes_reader/binary_file.h"

namespace dtack 
{ 
    namespace base
    {
        namespace bytes_reader
        {
            // ----------------------------------------------------------------
            binary_file_c::binary_file_c(dtack::base::environment_c *environment):
                base_c(environment)
            {
				// Adopt the environment's log consumer.
				set_log_consumer(environment->get_log_consumer()); set_log_level(environment->get_log_level());
            }

            // ----------------------------------------------------------------
            binary_file_c::~binary_file_c()
            {
				DTACK_COMMON_FUNCTION;

				log(LOG_LEVEL_DEBUG, "%s: destructing", F);

                close();
            }

            // ----------------------------------------------------------------
            void binary_file_c::open()
            {
                const char *filename = limn->peek("filename", std::string("binary.file")).c_str();

                stream.open(filename, std::ios::in | std::ios::binary);

                if (!stream.is_open())
                {
                    throw_runtime_error("unable to open %s: %s", filename, ::strerror(errno));
                }
    
            }

            // ----------------------------------------------------------------
            void binary_file_c::read(char *data, size_t size)
            {
                // Read the specified amount of data from the file.
                stream.read(data, size);

                // File did not have that many bytes left in it?
                if (stream.gcount() < (std::streamsize)size)
                {
                    if (stream.eof())
                        throw eof_exception_c();
                }
            }

            // ----------------------------------------------------------------
            void binary_file_c::close()
            {
                if (stream.is_open())
                    stream.close();
            }

        } // end namespace

    } // end namespace
    
} // end namespace