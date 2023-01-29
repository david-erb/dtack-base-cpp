#include <cstring>
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
            namespace asserts
            {
                class Main: public testing::Main
                {
                    public:
                        Main(dtack::base::environment_c *environment);
                        ~Main();
                        virtual int summarize_to_stderr();
                        virtual void run_configured_tests();
                        virtual void test1();
                        virtual void test2();
                        void write_file(const char *filename, void *data, int size);
                        void read_file(const char *filename, void **data, int *size);

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
                    /// This runs the two tests: {test1, test2}.  Both are always run.
                    /// The test::Main base class assert methods don't return an error or raise an exception.
                    /// Instead they just increment the instance variables pass_count and fail_count.
                    /// The output of this program is strange, because it will say FAIL in red during the assertions which fail on purpose.
                    /// The only real assertions are the ones assert_equal(F, "unexpected count", "0", ...
                    /// If either of these fail, then the whole test will fail because fail_count will be left non-zero.
                    ///
                    
                    DTACK_COMMON_FUNCTION;

                    // Set ourselves as the log consumer.
                    set_log_consumer(this);

                    // Set the level at which we will send log entries to the consumer.
                    set_log_level(configuration->peek_log_level());

                    const char *doing = "running";
                    try
                    {
                        doing = "test1";
                        test1();
                        doing = "test2";
                        test2();
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
                    
                } // end main
                    
                // ----------------------------------------------------------------
                void Main::test1()
                {
                    ///
                    /// Creates a Limn from pokes.  Then uses some assert methods on testing::Main to check the values.
                    ///

                    DTACK_COMMON_FUNCTION;
                    int unexpected_count = 0;

                    string a = "123";
                    string b = "123";
                    string c = "456";
                    string d = "789";
                    string e = "abc";
                    assert_equal(F, "a==b", a, b);

                    // Make first Limn.
                    Limn *actual = new_Limn(environment);
                    actual->poke("a", a);
                    actual->poke("c", c);
                    actual->poke("d", d);

                    // Make second Limn.
                    Limn *expect = new_Limn(environment);
                    expect->poke("a", a);
                    expect->poke("c", c);

                    // Reset the instance variables.
                    pass_count = fail_count = 0;
                    // Do the assertion, there should be two passes but no fails because expect is a subset of actual.
                    assert_contains_at_least(F, "limn comparison 1", actual, expect);
                    unexpected_count += pass_count != 2 || fail_count != 0;
                    assert_equal(F, "limn comparison 1 pass_count", "2", to_string(pass_count));
                    assert_equal(F, "limn comparison 1 fail_count", "0", to_string(fail_count));

                    pass_count = fail_count = 0;
                    // Do the assertion, which will increment the pass_count by 2 and the fail_count by 1.
                    // This is because actual is NOT a subset of expect.
                    assert_contains_at_least(F, "limn comparison 2", expect, actual);
                    unexpected_count += pass_count != 2 || fail_count != 1;
                    assert_equal(F, "limn comparison 2 pass_count", "2", to_string(pass_count));
                    assert_equal(F, "limn comparison 2 fail_count", "1", to_string(fail_count));

                    expect->poke("d", e);
                    pass_count = fail_count = 0;
                    // Do the assertion, which will increment the pass_count by 2 and the fail_count by 1.
                    // This is because with the addition of "d", expect is no longer a subset of actual.
                    assert_contains_at_least(F, "limn comparison 3", actual, expect);
                    unexpected_count += pass_count != 2 || fail_count != 1;
                    assert_equal(F, "limn comparison 3 pass_count", "2", to_string(pass_count));
                    assert_equal(F, "limn comparison 3 fail_count", "1", to_string(fail_count));

                    // Nothing unexpected should have happened.
                    pass_count = fail_count = 0;
                    assert_equal(F, "unexpected count", "0", to_string(unexpected_count));
                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test2()
                {
                    DTACK_COMMON_FUNCTION;
                    int unexpected_count = 0;

                    char data[1024];
                    data[0] = '\0';
                    strcat(data, "line1\n");
                    strcat(data, "line2\n");
                    strcat(data, "line3\n");
                    int size = strlen(data);

                    string directory = "/tmp/dtack-base-cpp/test_asserts";
                    string filename1 = directory + "/text1.txt";
                    string filename2 = directory + "/text2.txt";
                    string filename3 = directory + "/text3.txt";
                    string filename4 = directory + "/text4.txt";

                    int rc = mkdir_p(directory.c_str(), 0777);
                    if (rc == -1)
                        throw_runtime_error("unable to mkdir %s", directory.c_str());

                    dtack::base::write_file(filename1.c_str(), data, size);

                    // File2 same as file1.
                    dtack::base::write_file(filename2.c_str(), data, size);

                    // File3 differs in length.
                    dtack::base::write_file(filename3.c_str(), data, size-1);

                    // File4 differs at position 5.
                    ((char *)data)[5] = 'X';
                    dtack::base::write_file(filename4.c_str(), data, size);

                    // Do the assertion, which will increment the pass_count but no the fail_count.
                    assert_files(F, "file2", filename1.c_str(), filename2.c_str());
                    unexpected_count += pass_count != 0 || fail_count != 1;
                    assert_equal(F, "file2 pass_count", "1", to_string(pass_count));
                    assert_equal(F, "file2 fail_count", "0", to_string(fail_count));

                    pass_count = fail_count = 0;
                    // Do the assertion, which will increment the fail_count by 1 because the files are of different length.
                    assert_files(F, "file3", filename1.c_str(), filename3.c_str());
                    unexpected_count += pass_count != 0 || fail_count != 1;
                    assert_equal(F, "file3 pass_count", "0", to_string(pass_count));
                    assert_equal(F, "file3 fail_count", "1", to_string(fail_count));

                    pass_count = fail_count = 0;
                    // Do the assertion, which will increment the fail_count by 1 because the files have different contents.
                    assert_files(F, "file3", filename1.c_str(), filename4.c_str());
                    unexpected_count += pass_count != 0 || fail_count != 1;
                    assert_equal(F, "file4 pass_count", "0", to_string(pass_count));
                    assert_equal(F, "file4 fail_count", "1", to_string(fail_count));

                    // Nothing unexpected should have happened.
                    pass_count = fail_count = 0;
                    assert_equal(F, "unexpected count", "0", to_string(unexpected_count));

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
    dtack::base::tests::asserts::Main main(&environment);

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
