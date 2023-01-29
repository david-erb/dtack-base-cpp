#pragma once

#define DTACK_COMMON__S1(x) #x
#define DTACK_COMMON__S2(x) DTACK_COMMON__S1(x)
#define DTACK_COMMON_LOCATION_FORMAT "%s in %s@%s"
#define DTACK_COMMON_LOCATION_ARGS extract_class_and_method_name(std::string(__PRETTY_FUNCTION__)).c_str(), __FILE__, DTACK_COMMON__S2(__LINE__)
#define DTACK_COMMON_FUNCTION std::string F_string = extract_class_and_method_name(std::string(__PRETTY_FUNCTION__)); const char *F = F_string.c_str();

namespace dtack
{
    namespace base
    {
        // --------------------------------------------------------------------
        class diwec_log_consumer_i
        {
            public: virtual void consume_log(int level, const char *message) = 0;
        };

        // --------------------------------------------------------------------
        class diwec_log_producer_i
        {
            public:
                virtual void set_log_consumer(diwec_log_consumer_i *log_consumer) = 0;
                virtual void set_log_level(int log_level) = 0;
                virtual void set_log_level(const char *log_level_label) = 0;
                virtual void set_log_level(std::string log_level_label) = 0;
                virtual diwec_log_consumer_i *get_log_consumer() = 0;
                virtual int get_log_level() = 0;
        };

        // --------------------------------------------------------------------
        class diwec_log_producer_c
        {
            public:
                virtual void set_log_consumer(diwec_log_consumer_i *log_consumer);
                virtual void set_log_level(int log_level);
                virtual void set_log_level(const char *log_level_label);
                virtual void set_log_level(std::string log_level_label);
                virtual diwec_log_consumer_i *get_log_consumer();
                virtual int get_log_level();
                virtual void log(int level, const char *format, ...);
                virtual void log_elapsed(const char *what, int count, double elapsed, const char *warning);
                virtual void log_operations(const char *what, int count, double elapsed, const char *warning);
                static std::string get_log_level_label(int level);
                static int parse_log_level_label(std::string label);

            public:
                const static int LOG_LEVEL_CRITICAL = (50);
                const static int LOG_LEVEL_ERROR = (40);
                const static int LOG_LEVEL_WARNING = (30);
                const static int LOG_LEVEL_INFO = (20);
                const static int LOG_LEVEL_DEBUG = (10);
                const static int LOG_LEVEL_NOTSET = (0);

            private:
                diwec_log_consumer_i *log_consumer = NULL;
                int log_level = 0;
        };
    } // end namespace

} // end namespace
