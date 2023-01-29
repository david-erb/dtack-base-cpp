#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <fstream>
#include <chrono>
#include <thread>

#include <regex>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.configuration.h>
#include <dtack.base.limn.h>
#include <dtack.base.cli.h>

#include <dtack.base.string_token.h>
#include <dtack.base.string_walker.h>
#include <dtack.base.packet.h>

#include <dtack.base.testing.h>

using namespace dtack::base;

#define PROGRESS diwec_log_producer_c::LOG_LEVEL_INFO
#define ERROR diwec_log_producer_c::LOG_LEVEL_ERROR

namespace dtack
{
    namespace base
    {
        namespace tests
        {
            namespace configuration
            {
                class Main: public testing::Main
                {
                    public:
                        Main(dtack::base::environment_c *environment);
                        ~Main();
                        int summarize_to_stderr();
                        virtual void run_configured_tests();
                        virtual void test_configurations();
                        void test_configuration1();

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
                    ///
                    /// This runs the just one test: test_configurations().
                    /// The test::Main base class assert methods don't return an error or raise an exception.
                    /// Instead they just increment the instance variables pass_count and fail_count.
                    /// If fail_count is left at 0 when the function returns, the whole test will fail.
                    ///

                    // Set ourselves as the log consumer.
                    set_log_consumer(this);

                    // Set the level at which we will send log entries to the consumer.
                    set_log_level(configuration->peek_log_level());

                    const char *doing = "running";
                    try
                    {
                        doing = "calling test_configurations";
                        test_configurations();
                    }
                    catch(exception &exception)
                    {
                        log(ERROR, "exception in %s while %s: %s", __func__, doing, exception.what());
                        unexpected_exception_count++;
                    }
                    catch (...)
                    {
                        log(ERROR, "exception in %s while %s: %s", __func__, doing);
                        unexpected_exception_count++;
                    }
                    
                } // end main
                    
                // ----------------------------------------------------------------
                void Main::test_configurations()
                {
                    const char *doing = "running";

                    test_configuration1();

                    // assert_equal_quiet(__func__, "json_string", expected, json_string);

                    //printf("results_packet json is %d\n-------------------\n%s\n-------------------\n", (int)json_string.size(), json_string.c_str());

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test_configuration1()
                {
                    const char *doing = "running";

                    // Make a factory for the configuration.
                    configuration_factory_c configuration_factory(environment);

                    // Make a configuration.
                    shared_ptr<configuration_interface> my_configuration(configuration_factory.manufacture());

                    // Make a string which to parse.
                    string json_string =
                        "{"
                        "  \"test_packet1\": {"
                        "    \"root\": {"
                        "      \"inner\": {"
                        "        \"attributes\": {"
                        "        \"attrib1\": \"value1\""
                        "        },"
                        "        \"scalar1\": \"value2\","
                        "        \"inner\": {"
                        "          \"scalar2\": \"value3\""
                        "          }"
                        "        }"
                        "      }"
                        "    }"
                        "  ,"
                        "  \"test_packet2\": {"
                        "    \"parsed\": {"
                        "      \"scalar2\": \"quoted value\","
                        "      \"scalar1\": \"value1\","
                        "      \"level2\": {"
                        "        \"level3\": {"
                        "          \"scalar3\": \"value3\""
                        "          }"
                        "        }"
                        "      }"
                        "    }"
                        "  ,"
                        "  \"test_packet3\": {"
                        "    \"parsed\": {"
                        "      \"scalar2\": \"value2\","
                        "      \"scalar1\": \"value1\","
                        "      \"array1\": {"
                        "        \"item00000001\": \"value3\","
                        "        \"item00000000\": \"quoted value\""
                        "        }"
                        "      }"
                        "    }"
                        "  }";

                    // Load from the json string.
                    my_configuration->load_from_json_string(json_string);

                    // Check the expected values.
                    assert_equal(__func__, "scalar1", my_configuration->peek("test_packet3.parsed.scalar1", ""), "value1");
                    assert_equal(__func__, "array1[0]", my_configuration->peek("test_packet3.parsed.array1.item00000000", ""), "quoted value");
                    assert_equal(__func__, "array1[1]", my_configuration->peek("test_packet3.parsed.array1.item00000001", ""), "value3");
                    assert_equal(__func__, "scalar2", my_configuration->peek("test_packet3.parsed.scalar2", ""), "value2");

                    // Reference subsections within the json.
                    shared_ptr<configuration_interface> test_packet3_section(my_configuration->section("test_packet3"));
                    shared_ptr<configuration_interface> test_packet3_parsed_section(test_packet3_section->section("parsed"));

                    // Check the expected values.
                    assert_equal(__func__, "scalar1", test_packet3_parsed_section->peek("scalar1", ""), "value1");
                    assert_equal(__func__, "array1[0]", test_packet3_parsed_section->peek("array1.item00000000", ""), "quoted value");
                    assert_equal(__func__, "array1[1]", test_packet3_parsed_section->peek("array1.item00000001", ""), "value3");
                    assert_equal(__func__, "scalar2", test_packet3_parsed_section->peek("scalar2", ""), "value2");

                        // string json_string;
                        // parsed_packet->compose_jso_string(json_string);
                        // printf("parsed json is\n-------------------\n%s\n-------------------\n", json_string.c_str());

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
    dtack::base::tests::configuration::Main main(&environment);

    int return_code = 0;

    try
    {
        main.configuration->configure(argc, argv);

        // Get initial memory usage.
        double initial_vm_usage;
        double initial_resident_set;
        get_process_memory_usage(initial_vm_usage, initial_resident_set);

        // Loop to check for memory leak.
        int loop_count = 1;
        for (int loop = 0; loop < loop_count; loop++)
        {
            // Run the tests.
            main.run_configured_tests();

            double vm_usage;
            double resident_set;

            get_process_memory_usage(vm_usage, resident_set);

            printf(".......... %05d. %0.0f %0.0f delta %0.0f %0.0f\n", 
                loop,
                vm_usage, 
                resident_set,
                vm_usage - initial_vm_usage,
                resident_set - initial_resident_set);
        }

        return_code = main.summarize_to_stderr();
    }
    catch(const exception &exception)
    {
        fprintf(stderr, "%s\n", exception.what());
        return_code = -1;
    }

	exit(return_code);
}
