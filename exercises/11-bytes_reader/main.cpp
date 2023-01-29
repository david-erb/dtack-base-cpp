#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <unordered_map>

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.configuration.h>
#include <dtack.base.cli.h>

#include <dtack.base.bytes_reader.h>

#include <fstream>
#include <chrono>
#include <thread>

#define PROGRESS dtack::base::diwec_log_producer_c::LOG_LEVEL_INFO
#define REPORT dtack::base::diwec_log_producer_c::LOG_LEVEL_INFO
#define ERROR dtack::base::diwec_log_producer_c::LOG_LEVEL_ERROR

namespace dtack
{
    namespace exercises
    {

        class Main: public dtack::base::cli_c
        {
            public:
                Main(dtack::base::environment_c *environment);
                ~Main();
                virtual void run();
                virtual void read_test_file();
                virtual void report();

            private:
                dtack::base::bytes_reader::interface_i *bytes_reader = NULL;

        }; // end class

        // ----------------------------------------------------------------
        Main::Main(dtack::base::environment_c *environment):
            cli_c(environment)
        {
        } // end method
        // ----------------------------------------------------------------
        void Main::run()
        {
            const char *doing = "entering";

            try
            {
                doing = "reading test file";
                read_test_file();

                doing = "reporting";
                report();
            }
            catch(std::exception &exception)
            {
                log(ERROR, "exception in %s while %s: %s", __func__, doing, exception.what());
                throw;
            }
            catch (...)
            {
                log(ERROR, "exception in %s while %s: %s", __func__, doing);
                throw_runtime_error("unknown exception");
            }
            
        } // end main
        
        // ----------------------------------------------------------------
        void Main::read_test_file()
        {
            const char *doing = "entering";
            log(PROGRESS, "%s", doing);

            try
            {
                // Reference bytes_reader section from the configuration file.
                dtack::base::configuration_interface *bytes_reader_configuration = configuration->section("bytes_reader");

                // Reference ettings section from the previous section.
                dtack::base::configuration_interface *bytes_reader_settings = bytes_reader_configuration->section("settings");

                doing = "manufacturing";
                log(PROGRESS, "%s", doing);
                
                // Make a factory.
                dtack::base::bytes_reader::factory_c *factory = 
                    new dtack::base::bytes_reader::factory_c(environment);

                // Type of bytes_reader we want.
                std::string bytes_reader_type = bytes_reader_configuration->peek("type", std::string("binary_file"));
                
                // Make the bytes_reader instance.
                bytes_reader = factory->manufacture(bytes_reader_type);

                doing = "configuring";

                // Stuff the input filename into the configuration.
                bytes_reader_settings->poke("filename", "exercises/11-bytes_reader/binary_file.dat");

                // Configure the bytes_reader instance.
                bytes_reader->configure(bytes_reader_settings);

                doing = "opening";

                bytes_reader->open();

                doing = "reading";

                int size = 1024 * 100;
                char *data = (char *)allocate_from_heap(__func__, "buffer_data", size, 1);

                bytes_reader->read(data, size=4);
                log(PROGRESS, "read %d bytes: \"%s\"", size, (char *)data);

                bytes_reader->read(data, size=14);
                log(PROGRESS, "read %d bytes: \"%s\"", size, (char *)data);

                try
                {
                    bytes_reader->read(data, size=1);
                    log(PROGRESS, "read %d bytes: \"%s\"", size, (char *)data);
                }
                catch(dtack::base::bytes_reader::eof_exception_c &exception)
                {
                    log(PROGRESS, "unable to read any more bytes");
                }

                doing = "closing";

                bytes_reader->close();

                delete bytes_reader;

                bytes_reader = NULL;

            }
            catch(std::exception &exception)
            {
                log(ERROR, "exception in %s while %s: %s", __func__, doing, exception.what());
                throw;
            }
            catch (...)
            {
                log(ERROR, "exception in %s while %s: %s", __func__, doing);
                throw_runtime_error("unknown exception");
            }
            
        } // end method

        // ----------------------------------------------------------------
        void Main::report()
        {
            const char *doing = "entering";

            try
            {
            }
            catch(std::exception &exception)
            {
                log(ERROR, "exception in %s while %s: %s", __func__, doing, exception.what());
                throw;
            }
            catch (...)
            {
                log(ERROR, "exception in %s while %s: %s", __func__, doing);
                throw_runtime_error("unknown exception");
            }
            
        } // end method

        // ----------------------------------------------------------------
        Main::~Main()
        {
            if (bytes_reader != NULL)
            {
                bytes_reader->close();
                delete bytes_reader;
            }
        } // end method

    } // end namespace

} // end namespace

// --------------------------------------------------------------------
int main(int argc, char **argv)
{
    dtack::base::environment_c environment;
    dtack::exercises::Main main(&environment);

    int exit_code = 0;

    try
    {
        main.configuration->configure(argc, argv);

        main.run();
    }
    catch(const std::exception &exception)
    {
        fprintf(stderr, "%s\n", exception.what());
        exit_code = -1;
    }

	exit(exit_code);
}
