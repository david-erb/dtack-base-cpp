#include <cstring>
#include <regex>
#include <stdarg.h>
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

namespace dtack
{
    namespace base
    {
        namespace testing
        {
            // These are ANSI escape sequences.
            // See https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
            const char *console_green = "\033[0;92m";
            const char *console_nocolor = "\033[0m";
            const char *console_red = "\033[0;91m";

            // ----------------------------------------------------------------
            Main::Main(dtack::base::environment_c *environment):
                cli_c(environment)
            {

            }
            
            // ----------------------------------------------------------------
            Main::~Main()
            {

            }

            // ----------------------------------------------------------------
            int Main::summarize_to_stderr(const char *F)
            {
                if (unexpected_exception_count != 0)
                {
                    fprintf(stderr, "%s: %d unexpected exception%s happened\n", F, unexpected_exception_count, unexpected_exception_count==1? "": "s");
                    return 1;
                }
                 
                if (fail_count != 0)
                {
                    fprintf(stderr, "%s: %d assertion%s failed\n", F, fail_count, fail_count==1? "": "s");
                    return 1;
                }
                 
                return 0;

            } // end method

            // ----------------------------------------------------------------
            void Main::assert_match(
                const char *caller, 
                const char *what, 
                std::string a, 
                std::string b)
            {
                bool a_is_regex = (a[0] == '/' && a[a.length()-1] == '/');
                bool b_is_regex = (b[0] == '/' && b[b.length()-1] == '/');

                if (a_is_regex ^ b_is_regex)
                {
                    if (a_is_regex)
                    {
                        a = a.substr(1, a.length()-2);
                        const std::regex regex(a);
                        if (std::regex_match(b, regex))
                            report_pass(caller, what, "\"%s\"", b.c_str());
                        else
                            report_fail(caller, what, "\"%s\" ~= a \"%s\"", b.c_str(), a.c_str());
                    }
                    else
                    {
                        b = b.substr(1, b.length()-2);
                        const std::regex regex(b);
                        if (std::regex_match(a, regex))
                            report_pass(caller, what, "\"%s\"", a.c_str());
                        else
                            report_fail(caller, what, "\"%s\" ~= b \"%s\"", a.c_str(), b.c_str());
                    }
                    
                }
                else
                {
                    assert_equal(caller, what, a, b);
                }
                

            } // end method
                
            // ----------------------------------------------------------------
            void Main::assert_equal(
                const char *caller, 
                const char *what, 
                std::string a, 
                std::string b)
            {
                if (a == b)
                    report_pass(caller, what, "\"%s\"", a.c_str());
                else
                    report_fail(caller, what, "\"%s\" == \"%s\"", a.c_str(), b.c_str());

            } // end method
                
            // ----------------------------------------------------------------
            void Main::assert_equal(
                const char *caller, 
                const char *what, 
                const void *a_data, 
                int a_size,
                const void *b_data,
                int b_size)
            {
                if (a_size != b_size)
                {
                    report_fail(caller, what, "size %d == %d", a_size, b_size);
                }
                else
                {
                    char a_char;
                    char b_char;
                    int i;
                    for (i=0; i<a_size; i++)
                    {
                        a_char = ((char *)a_data)[i];
                        b_char = ((char *)b_data)[i]; 
                        if (a_char != b_char)
                            break;
                    }

                    if (i == a_size)
                    {
                        report_pass(caller, what, "%d bytes equal", a_size);
                    }
                    else
                    {
                        report_fail(caller, what, "char[%d] 0x%02x == 0x%02x", i, (int)a_char & 0xff, (int)b_char & 0xff);
                    }
                }
            } // end method
                
            // ----------------------------------------------------------------
            void Main::assert_equal_quiet(
                const char *caller, 
                const char *what, 
                std::string a, 
                std::string b)
            {
                if (a == b)
                    report_pass(caller, what, "expected", a.c_str(), b.c_str());
                else
                {
                    report_fail(caller, what, "expected", a.c_str(), b.c_str());
                    log(LOG_LEVEL_DEBUG, "%s: [%s] expected %s", caller, what, a.c_str());
                    log(LOG_LEVEL_DEBUG, "%s: [%s] but got %s", caller, what, b.c_str());
                    
                }

            } // end method

            // ----------------------------------------------------------------
            void Main::assert_equal(
                const char *caller, 
                const char *what, 
                size_t a, 
                size_t b)
            {
                if (a == b)
                    report_pass(caller, what, "%zd", a);
                else
                    report_fail(caller, what, "%zd == %zd", a, b);

            } // end method
                
            // ----------------------------------------------------------------
            void Main::assert(
                const char *caller, 
                const char *what,
                const char *reason,
                bool truth)
            {
                if (truth)
                    report_pass(caller, what, reason);
                else
                {
                    report_fail(caller, what, reason);
                    
                }

            } // end method
                
            // ----------------------------------------------------------------
            void Main::report_pass(
                const char *caller, 
                const char *what, 
                const char *format, 
                ...)
            {
                va_list args;
                va_start(args, format);
                std::string message = printf_to_string_va(format, args);
                va_end(args);

                if (message != "")
                    report(caller, what, "%s... %sPASS%s", message.c_str(), console_green, console_nocolor);
                else
                    report(caller, what, "... %sPASS%s", console_green, console_nocolor);

                pass_count++;
            } // end method
                
            // ----------------------------------------------------------------
            void Main::report_fail(
                const char *caller, 
                const char *what, 
                const char *format, 
                ...)
            {
                va_list args;
                va_start(args, format);
                std::string message = printf_to_string_va(format, args);
                va_end(args);


                if (message != "")
                    report(caller, what, "%s... %sFAIL%s", message.c_str(), console_red, console_nocolor);
                else
                    report(caller, what, "... %sFAIL%s", console_red, console_nocolor);

                fail_count++;
            } // end method
                
            // ----------------------------------------------------------------
            void Main::report(
                const char *caller, 
                const char *what, 
                const char *format, 
                ...)
            {
                va_list args;
                va_start(args, format);
                std::string message = printf_to_string_va(format, args);
                va_end(args);

                log(LOG_LEVEL_CRITICAL, "%s: [%s] %s", caller, what, message.c_str());

            } // end method

			// ----------------------------------------------------------------
			void Main::assert_contains_at_least(
                const char *caller, 
				const char *what,
				Limn *actual, 
				string expect_json)
			{
				Limn *expect = new_Limn(environment);
				expect->load_from_json_string(expect_json);

                assert_contains_at_least(caller, what, actual, expect);

				delete expect;
			} // end method
            
			// ----------------------------------------------------------------
			void Main::assert_contains_at_least(
                const char *caller, 
				const char *what,
				Limn *actual, 
				Limn *expect)
			{
				vector<string> actual_scalar_names = actual->get_scalar_names();
				vector<string> expect_scalar_names = expect->get_scalar_names();

				for (auto expect_scalar_name : expect_scalar_names)
				{
                    string what2;
                    if (strlen(what) != 0)
                        what2 = printf_to_string("%s %s", what, expect_scalar_name.c_str());
                    else
                        what2 = expect_scalar_name;
					if (find(actual_scalar_names.begin(), actual_scalar_names.end(), expect_scalar_name) != actual_scalar_names.end())
					{
						assert_match(caller, what2.c_str(), 
							actual->peek(expect_scalar_name.c_str(), ""),
							expect->peek(expect_scalar_name.c_str(), ""));
					}
					else
					{
						assert(caller, what2.c_str(), "present", false);
					}
				}
			} // end method

            // ----------------------------------------------------------------
            void Main::assert_files(
                const char *caller, 
				const char *what,
                const char *actual_filename, 
                const char *expect_filename)
            {
                void *actual_data;
                int actual_size;

                try
                {
                    dtack::base::read_file(actual_filename, &actual_data, &actual_size);
                }
                catch(const std::exception& e)
                {
                    assert(caller, what, printf_to_string("actual file %s can be read", actual_filename).c_str(), false);
                    return;
                }

                void *expect_data;
                int expect_size;

                try
                {
                    dtack::base::read_file(expect_filename, &expect_data, &expect_size);
                }
                catch(const std::exception& e)
                {
                    assert(caller, what, printf_to_string("expect file %s can be read", expect_filename).c_str(), false);
                    free(actual_data);
                    return;
                }

                // printf(".......... %s@%d: %s size is %d\n", __FILE__, __LINE__, actual_filename, actual_size);
                // printf(".......... %s@%d: %s size is %d\n", __FILE__, __LINE__, expect_filename, expect_size);
                
                assert_equal(caller, what, actual_data, actual_size, expect_data, expect_size);

                free(expect_data);
                free(actual_data);

            } // end method

        } // end namespace

    } // end namespace

} // end namespace