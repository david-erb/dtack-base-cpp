#include <string>
#include <regex>
#include <stdarg.h>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>
using namespace std;

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.configuration.h>
#include <dtack.base.limn.h>
#include <dtack.base.cli.h>

#include <dtack.base.incidents.h>

#include <dtack.base.testing.h>

using namespace dtack::base;

namespace dtack
{
    namespace base
    {
		// -----------------------------------------------------------

        namespace tests
        {
            namespace version
            {
                class Main: public testing::Main
                {
                    public:
                        Main(dtack::base::environment_c *environment);
                        ~Main();
                        virtual int summarize_to_stderr();
                        virtual void run_configured_tests();

                }; // end class

                // ----------------------------------------------------------------
                Main::Main(dtack::base::environment_c *environment):
                    testing::Main(environment)
                {

                } // end method

                // ----------------------------------------------------------------
                Main::~Main()
                {

                } // end method

                // ----------------------------------------------------------------
                int Main::summarize_to_stderr()
                {
                    DTACK_COMMON_FUNCTION;

                    return testing::Main::summarize_to_stderr(F);
                } // end method

                // ----------------------------------------------------------------
                void Main::run_configured_tests()
                {
                    DTACK_COMMON_FUNCTION;

                    // Set ourselves as the log consumer.
                    set_log_consumer(this);

                    // Set the level at which we will send log entries to the consumer.
                    set_log_level(configuration->peek_log_level());

                    const char *doing = "running";
                    try
                    {
                        assert("main", "version", dtack::base::version().to_string().c_str(), true);
                    }
                    catch(exception &exception)
                    {
                        log(LOG_LEVEL_ERROR, "%s: exception while %s: %s", F, doing, exception.what());
                        unexpected_exception_count++;
                    }
                    catch (...)
                    {
                        log(LOG_LEVEL_ERROR, "%s: exception while %s", F, doing);
                        unexpected_exception_count++;
                    }
                    
                } // end method
                                        
            } // end namespace

        } // end namespace

    } // end namespace

} // end namespace

using namespace dtack::base;

// --------------------------------------------------------------------
int main(int argc, char **argv)
{
    environment_c environment;
    dtack::base::tests::version::Main main(&environment);

    int return_code = 0;

    try
    {
        main.configuration->configure(argc, argv);

        main.run_configured_tests();
        
        return_code = main.summarize_to_stderr();
    }
    catch(const exception &exception)
    {
        fprintf(stderr, "%s\n", exception.what());
        return_code = -1;
    }

	exit(return_code);
}
