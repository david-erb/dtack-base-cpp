#pragma once

namespace dtack
{
    namespace base
    {
        class cli_c: public base_c, public diwec_log_producer_c, public diwec_log_consumer_i
        {
            public:
                cli_c(environment_c *environment);
                ~cli_c();
                configuration_interface *configuration;
                Limn *limn;

                // This satisfies the log_consumer interface.
                void consume_log(int level, const char *message);

        }; // end class

    } // end namespace

} // end namespace