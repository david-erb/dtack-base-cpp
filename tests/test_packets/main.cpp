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
            namespace packets
            {
                class Main: public testing::Main
                {
                    public:
                        Main(dtack::base::environment_c *environment);
                        ~Main();
                        int summarize_to_stderr();
                        virtual void run_configured_tests();
                        virtual void test_packets();
                        void test_packet1(std::shared_ptr<packet_c> results_packet);
                        void test_packet2(std::shared_ptr<packet_c> results_packet);
                        void test_packet3(std::shared_ptr<packet_c> results_packet);

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
                    /// This runs the just one test: test_packets().
                    /// Packets act basically like python dictionaries.  The can have sub-packets and scalars.
                    /// Packets can also be read from json and composed into json.
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
                        doing = "calling test_packets";
                        test_packets();
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
                void Main::test_packets()
                {
                    const char *doing = "running";

                    // Make a root packet which accumulates the results.
                    std::shared_ptr<packet_c> results_packet = std::make_shared<packet_c>(environment, "results");

                    // Make some sub-packets in the results root.
                    // Each sub-packet has different contents poked into it.
                    test_packet1(results_packet->new_packet("test_packet1"));

                    test_packet2(results_packet->new_packet("test_packet2"));

                    test_packet3(results_packet->new_packet("test_packet3"));
                    
                    string json_string;
                    results_packet->options["indent"] = "yes";
                    results_packet->compose_jso_string(json_string);
                    //json_string.erase(remove(json_string.begin(), json_string.end(), '\n'), json_string.end());

                    // The results packet should look like this when composed to json.
                    string expected(
        "\"results\": {\n"
        "  \"test_packet1\": {\n"
        "    \"root\": {\n"
        "      \"inner\": {\n"
        "        \"attributes\": {\n"
        "        \"attrib1\": \"value1\"\n"
        "        },\n"
        "        \"scalar1\": \"value2\",\n"
        "        \"inner\": {\n"
        "          \"scalar2\": \"value3\"\n"
        "          }\n"
        "        }\n"
        "      }\n"
        "    }\n"
        "  ,\n"
        "  \"test_packet2\": {\n"
        "    \"parsed\": {\n"
        "      \"scalar2\": \"quoted value\",\n"
        "      \"scalar1\": \"value1\",\n"
        "      \"level2\": {\n"
        "        \"level3\": {\n"
        "          \"scalar3\": \"value3\"\n"
        "          }\n"
        "        }\n"
        "      }\n"
        "    }\n"
        "  ,\n"
        "  \"test_packet3\": {\n"
        "    \"parsed\": {\n"
        "      \"scalar2\": \"value2\",\n"
        "      \"scalar1\": \"value1\",\n"
        "      \"array1\": {\n"
        "        \"item00000001\": \"value3\",\n"
        "        \"item00000000\": \"quoted value\"\n"
        "        }\n"
        "      }\n"
        "    }\n"
        "  }\n");

                    assert_equal_quiet(__func__, "json_string", expected, json_string);

                    //printf("results_packet json is %d\n-------------------\n%s\n-------------------\n", (int)json_string.size(), json_string.c_str());

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test_packet1(std::shared_ptr<packet_c> results_packet)
                {
                    const char *doing = "running";

                    std::shared_ptr<packet_c> root_packet = results_packet->new_packet("root");
                    std::shared_ptr<packet_c> inner_packet = root_packet->new_packet("inner");
                    inner_packet->add_attrib("attrib1", "value1");
                    inner_packet->add_scalar("scalar1", "value2");

                    std::shared_ptr<packet_c> inner_inner_packet = inner_packet->new_packet("inner");
                    inner_inner_packet->add_scalar("scalar2", "value3");

                    assert_equal(__func__, "inner.scalar1", root_packet->peek_scalar("inner.scalar1"), "value2");
                    assert_equal(__func__, "inner.inner.scalar2", root_packet->peek_scalar("inner.inner.scalar2"), "value3");

                    string json_string;
                    // root_packet->options["indent"] = "no";
                    root_packet->compose_jso_string(json_string);

                    string expected = 
                    "\"root\": {\"inner\": {\"attributes\": {\"attrib1\": \"value1\"},\"scalar1\": \"value2\",\"inner\": {\"scalar2\": \"value3\"}}}";
                    assert_equal_quiet(__func__, "json_string", json_string, expected);

                } // end method
                    

                // ----------------------------------------------------------------
                void Main::test_packet2(std::shared_ptr<packet_c> results_packet)
                {
                    const char *doing = "running";

                    std::shared_ptr<packet_c> parsed_packet = results_packet->new_packet("parsed");

                    string json_string =
                        "{scalar1: value1, scalar2: \"quoted value\", level2: {level3: {scalar3: value3}}}";

                    parsed_packet->load_from_json_string(json_string);

                    assert_equal(__func__, "scalar1", parsed_packet->peek_scalar("scalar1"), "value1");
                    assert_equal(__func__, "scalar2", parsed_packet->peek_scalar("scalar2"), "quoted value");
                    assert_equal(__func__, "level2.level3.scalar3", parsed_packet->peek_scalar("level2.level3.scalar3"), "value3");

                        // string json_string;
                        // parsed_packet->compose_jso_string(json_string);
                        // printf("parsed json is\n-------------------\n%s\n-------------------\n", json_string.c_str());

                } // end method

                // ----------------------------------------------------------------
                void Main::test_packet3(std::shared_ptr<packet_c> results_packet)
                {
                    const char *doing = "running";

                    std::shared_ptr<packet_c> parsed_packet = results_packet->new_packet("parsed");

                    string json_string =
                        "{scalar1: value1, array1: [\"quoted value\", value3], scalar2: value2}";

                    parsed_packet->load_from_json_string(json_string);

                    assert_equal(__func__, "scalar1", parsed_packet->peek_scalar("scalar1"), "value1");
                    assert_equal(__func__, "array1[0]", parsed_packet->peek_scalar("array1.item00000000"), "quoted value");
                    assert_equal(__func__, "array1[1]", parsed_packet->peek_scalar("array1.item00000001"), "value3");
                    assert_equal(__func__, "scalar2", parsed_packet->peek_scalar("scalar2"), "value2");

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
    dtack::base::tests::packets::Main main(&environment);

    int return_code = 0;

    try
    {
        main.configuration->configure(argc, argv);

        double initial_vm_usage;
        double initial_resident_set;

        get_process_memory_usage(initial_vm_usage, initial_resident_set);

        // Ability to loop for memory leak checking.
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
