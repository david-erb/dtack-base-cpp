namespace dtack
{
    namespace base
    {
        namespace testing
        {
            class Main: public cli_c
            {
                public:
                    Main(dtack::base::environment_c *environment);
                    ~Main();

                    int summarize_to_stderr(const char *F);

                    void assert_match(
                        const char *caller, 
                        const char *what, 
                        std::string a, 
                        std::string b);
                    void assert_equal(
                        const char *caller, 
                        const char *what, 
                        const void *a_data, 
                        int a_size,
                        const void *b_data,
                        int b_size);
                    void assert_equal(
                        const char *caller, 
                        const char *what, 
                        string a, 
                        string b);
                    void assert_equal_quiet(
                        const char *caller, 
                        const char *what, 
                        string a, 
                        string b);
                    void assert_equal(
                        const char *caller, 
                        const char *what, 
                        size_t a, 
                        size_t b);
                    void assert(
                        const char *caller, 
                        const char *what,
                        const char *reason, 
                        bool truth);

                    void report_pass(
                        const char *caller, 
                        const char *what, 
                        const char *format, 
                        ...);
                    void report_fail(
                        const char *caller, 
                        const char *what, 
                        const char *format, 
                        ...);
                    void report(
                        const char *caller, 
                        const char *what, 
                        const char *format, 
                        ...);

        			void assert_contains_at_least(
                        const char *caller, 
		        		const char *what,
				        Limn *actual, 
        				string expect_json);
        			void assert_contains_at_least(
                        const char *caller, 
		        		const char *what,
				        Limn *actual, 
        				Limn *expect);

                    void assert_files(
                        const char *caller, 
                        const char *what,
                        const char *actual_filename, 
                        const char *expect_filename);

                public:
                    int unexpected_exception_count = 0;
                    int pass_count = 0;
                    int fail_count = 0;

            }; // end class

        } // end namespace

    } // end namespace

} // end namespace
