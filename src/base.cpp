#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <cstring>
#include <string>
#include <stdarg.h>
#include <stdexcept>
#include <fstream>
#include <unordered_map>

#include "dtack.base.diwec_logging.h"
#include <dtack.base.environment.h>
#include <dtack.base.base.h>

namespace dtack
{
	namespace base
	{
        // ------------------------------------------------------------
        base_c::base_c(environment_c *environment)
        {
            this->environment = environment;
        } // end constructor

        // ------------------------------------------------------------
        base_c::~base_c()
        {
			// printf(".......... %s@%d\n", __FILE__, __LINE__);
        } // end destructor

        // ------------------------------------------------------------
        std::string base_c::enter(std::string function_name)
        {
            return function_name;
        } // end method

        // ------------------------------------------------------------
        std::string base_c::require(std::unordered_map<std::string, std::string> &map, std::string &key)
        {
            if (!map.count(key))
                throw_runtime_error("map does not contain key \"%s\"", key.c_str());
            return map[key];
        } // end method

        // ------------------------------------------------------------
        std::string base_c::value(std::unordered_map<std::string, std::string> &map, const std::string &key, const std::string &default_value)
        {
            if (!map.count(key))
                return default_value;
            else
                return map[key];
        } // end method

        // ------------------------------------------------------------
        std::string base_c::value(std::unordered_map<std::string, std::string> &map, const char *key, const char *default_value)
        {
            if (!map.count(key))
                return std::string(default_value);
            else
                return map[key];
        } // end method

        // ------------------------------------------------------------
        std::string base_c::get_attribute(const char *key, const char *default_value)
        {
            return value(attributes, key, default_value == NULL? "": default_value);
        } // end method

        // ------------------------------------------------------------
        void base_c::set_attribute(const char *key, const char *value)
        {
            attributes[key] = value;
        } // end method

        // ------------------------------------------------------------
        std::string format_compiled_date(const char *compiled_date, const char *compiled_time)
        {
            char month_string[32];
            int month;
            int day;
            int year;
            sscanf(compiled_date, "%s %d %d", month_string, &day, &year);

            if (!strcasecmp("jan", month_string)) month = 1;
            if (!strcasecmp("feb", month_string)) month = 2;
            if (!strcasecmp("mar", month_string)) month = 3;
            if (!strcasecmp("apr", month_string)) month = 4;
            if (!strcasecmp("may", month_string)) month = 5;
            if (!strcasecmp("jun", month_string)) month = 6;
            if (!strcasecmp("jul", month_string)) month = 7;
            if (!strcasecmp("aug", month_string)) month = 8;
            if (!strcasecmp("sep", month_string)) month = 9;
            if (!strcasecmp("oct", month_string)) month = 10;
            if (!strcasecmp("nov", month_string)) month = 11;
            if (!strcasecmp("dec", month_string)) month = 12;

            return printf_to_string("%04d-%02d-%02d %s", year, month, day, compiled_time);
        } // end method

        // ----------------------------------------------------------------
        std::string extract_class_and_method_name(const std::string &prettyFunction)
        {
            
            // printf(".......... %s@%d pretty_function is \"%s\"\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);

            // size_t colons = prettyFunction.find("::");
            // size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
            // size_t end = prettyFunction.rfind("(") - begin;

            // size_t begin = prettyFunction.find("::");

            // begin = prettyFunction.rfind(" ", begin) + 1;
            // if (begin == std::string::npos)
            //     begin = 0;
            
            size_t end = prettyFunction.rfind("(");
            
            size_t begin = prettyFunction.rfind(" ", end);

            return prettyFunction.substr(begin+1, end-begin-1);
        } // end method

        // ----------------------------------------------------------------
        std::string escape_html(const std::string &source)
        {
            std::string s = source;
            s = replace(s, "&", "&amp;");
            s = replace(s, "'", "&#39;");
            s = replace(s, "\"", "&quot;");
            s = replace(s, "<", "&lt;");
            s = replace(s, ">", "&gt;");
            return s;
        }

        // ----------------------------------------------------------------
        std::string unescape_html(const std::string &source)
        {
            std::string s = source;
            s = replace(s, "&amp;", "&");
            s = replace(s, "&#39;", "'");
            s = replace(s, "&quot;", "\"");
            s = replace(s, "&lt;", "<");
            s = replace(s, "&gt;", ">");
            return s;
        }

        // ----------------------------------------------------------------
        std::string replace(std::string source, const std::string find, const std::string replace)
        {
            size_t pos = 0;
            while ((pos = source.find(find, pos)) != std::string::npos)
            {
                source.replace(pos, find.length(), replace);
                pos += replace.length();
            }
            return source;
        }

        // ----------------------------------------------------------------
        void *allocate_from_heap(const char *caller, const char *purpose, int count, int size)
        {
            void *p = calloc(count, size);
            if (p == NULL)
                throw_runtime_error("%s unable to calloc(%d, %d) for %s", caller, count, size, purpose);
            return p;
        }

        // ----------------------------------------------------------------
        // Return a string made from sprintf of the variable argument list.

        std::string printf_to_string(const char *format, ...)
        {
            
            va_list args;
            va_start(args, format);
            std::string new_string = printf_to_string_va(format, args);
            va_end(args);

            return new_string;
        }

        // ----------------------------------------------------------------
        // Return a string made from sprintf of the variable argument list.

        std::string printf_to_string_va(const char *format, va_list args)
        {
            std::string new_string;
            
            int length = 0;
            va_list args2;
            va_copy(args2, args);
            length = vsnprintf(NULL, 0, format, args2);
            va_end(args2);

            if (length > 0)
            {
                char *buffer = (char *)allocate_from_heap(__func__, "formatted string", length+1, sizeof(char));
                vsprintf(buffer, format, args);
                new_string = buffer;
                free(buffer);
            }

            return new_string;
        }

        // ----------------------------------------------------------------
        void throw_runtime_error(const char *format, ...)
        {
            int length = 0;
            {
                va_list args;
                va_start(args, format);
                length = vsnprintf(NULL, 0, format, args);
                va_end(args);
            }

            if (length > 1)
            {
                char *buffer = (char *)malloc(sizeof(char) * length+1);
                va_list args;
                va_start(args, format);
                vsprintf(buffer, format, args);
                std::runtime_error exception = std::runtime_error(buffer);
                free(buffer);
                throw exception;
            }
            else
            {
                throw std::runtime_error("(no reason given)");
            }
        } // end method

        // ----------------------------------------------------------------
        clock_seconds_t get_clock_seconds()
        {
            struct timespec now;
			timespec_get(&now, TIME_UTC);
            // clock_gettime(CLOCK_MONOTONIC, &now);
            return now.tv_sec + now.tv_nsec / 1000000000.0;
        } // end method

        
        // ----------------------------------------------------------------
        /* recursive mkdir based on
        http://nion.modprobe.de/blog/archives/357-Recursive-directory-creation.html
        */
        #define PATH_MAX_STRING_SIZE 256

        /* recursive mkdir */
        int mkdir_p(const char *dir, const mode_t mode) 
        {
            char tmp[PATH_MAX_STRING_SIZE];
            char *p = NULL;
            struct stat sb;
            size_t len;
            
            /* copy path */
            len = strnlen (dir, PATH_MAX_STRING_SIZE);
            if (len == 0 || len == PATH_MAX_STRING_SIZE) {
                return -1;
            }
            memcpy (tmp, dir, len);
            tmp[len] = '\0';

            /* remove trailing slash */
            if(tmp[len - 1] == '/') {
                tmp[len - 1] = '\0';
            }

            /* check if path exists and is a directory */
            if (stat (tmp, &sb) == 0) {
                if (S_ISDIR (sb.st_mode)) {
                    return 0;
                }
            }
            
            /* recursive mkdir */
            for(p = tmp + 1; *p; p++) {
                if(*p == '/') {
                    *p = 0;
                    /* test path */
                    if (stat(tmp, &sb) != 0) {
                        /* path does not exist - create directory */
                        if (mkdir(tmp, mode) < 0) {
                            return -1;
                        }
                    } else if (!S_ISDIR(sb.st_mode)) {
                        /* not a directory */
                        return -1;
                    }
                    *p = '/';
                }
            }
            /* test path */
            if (stat(tmp, &sb) != 0) {
                /* path does not exist - create directory */
                if (mkdir(tmp, mode) < 0) {
                    return -1;
                }
            } else if (!S_ISDIR(sb.st_mode)) {
                /* not a directory */
                return -1;
            }
            return 0;
        } // end function

        // ----------------------------------------------------------------
        void write_file(const char *filename, void *data, int size)
        {
            FILE *file = fopen(filename, "wb");
            if (file == NULL)
                throw_runtime_error("unable to open for writing %s", filename);

            int rc = fwrite(data, 1, size, file);

            fclose(file);

            if (rc == -1)
                throw_runtime_error("error writing %s", filename);

            if (rc < size)
                throw_runtime_error("only wrote %zd out of %zd bytes to %s", rc, size, filename);

        } // end method
            
        // ----------------------------------------------------------------
        void read_file(const char *filename, void **data, int *size)
        {
            FILE *file = fopen(filename, "rb");
            if (file == NULL)
                throw_runtime_error("unable to open for reading %s", filename);

            int rc;
            rc = fseek(file, 0, SEEK_END);
            if (rc == -1)
                throw_runtime_error("error fseek(SEEK_END) %s", filename);

            rc = ftell(file);
            if (rc == -1)
                throw_runtime_error("error ftell() %s", filename);

            *size = rc;

            rc = fseek(file, 0, SEEK_SET);
            if (rc == -1)
                throw_runtime_error("error fseek(SEEK_SET) %s", filename);

            *data = allocate_from_heap("read_file", "file data", *size, 1);
            rc = fread(*data, 1, *size, file);
            if (rc == -1)
                throw_runtime_error("error reading from %s", filename);
            if (rc < *size)
                throw_runtime_error("only read %d out of %d bytes from %s", rc, *size, filename);

            fclose(file);

        } // end method

        // ----------------------------------------------------------------
        void get_process_memory_usage(double& vm_usage, double& resident_set)
        {
            vm_usage     = 0.0;
            resident_set = 0.0;

            // the two fields we want
            unsigned long vsize;
            long rss;
            {
                std::string ignore;
                std::ifstream ifs("/proc/self/stat", std::ios_base::in);
                ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                        >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                        >> ignore >> ignore >> vsize >> rss;
            }

            // long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
            // vm_usage = vsize / 1024.0;
            // resident_set = rss * page_size_kb;
            long page_size = sysconf(_SC_PAGE_SIZE); // in case x86-64 is configured to use 2MB pages
            vm_usage = vsize;
            resident_set = rss * page_size;
        } // end method

	} // end namespace

} // end namespace
