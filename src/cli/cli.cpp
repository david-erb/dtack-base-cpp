#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.configuration.h"
#include "dtack.base.limn.h"
#include "dtack.base.cli.h"

#include <chrono>
#include <thread>

namespace dtack
{
    namespace base
    {

        // ----------------------------------------------------------------
        cli_c::cli_c(environment_c *environment):
            base_c(environment),
            configuration(NULL),
            limn(NULL)
        {
            // As the command line interface, we are prepared to handle the standard environment logging.
			environment->set_log_consumer(this);

            // These are deprecated in favor of limn.
            configuration_factory_c configuration_factory(environment);
            configuration = configuration_factory.manufacture();

            // Get a limn of the default implementation type.
            limn = new_Limn(environment);

            // We will consume our own logs.
            set_log_consumer(this);
        } // end method

        // ----------------------------------------------------------------
        cli_c::~cli_c()
        {
            if (limn != NULL)
            {
                // printf("~cli_c: deleting limn 0x%08x\n", limn);
                delete limn;
            }
            
            if (configuration != NULL)
            {
                // printf("~cli_c: deleting configuration 0x%08x\n", configuration);
                delete configuration;
            }
        } // end method

        //--------------------------------------------------------
        // Logging produced from inside the diwec_log_producer_c class and from the LC400_RC_THROW macro.
        // This method makes this class eligible to be a diwec_log_consumer_i.

        void cli_c::consume_log(int level, const char *message)
        {

            struct timeval tv;
            gettimeofday(&tv, NULL);

            struct tm* tm_info;
            tm_info = std::localtime(&tv.tv_sec);

            char formatted_date[256];
            strftime(formatted_date, 26, "%Y-%m-%d %H:%M:%S", tm_info);
            sprintf(formatted_date+strlen(formatted_date), ".%06ld", tv.tv_usec);

            // Pad the log level label to make for fixed columns.
            std::string label = diwec_log_producer_c::get_log_level_label(level);
            if (label.size() < 8)
                label.insert(label.size(), 8-label.size(), ' ');

            std::string decorated_message = std::string(formatted_date) + " " + label + " " + message;

            printf(decorated_message.c_str());
            printf("\n");
            fflush(stdout);

        } // end method

    } // end namespace

} // end namespace