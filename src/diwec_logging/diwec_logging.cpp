#include <chrono>
#include <math.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include "dtack.base.diwec_logging.h"

namespace dtack
{
    namespace base
    {
        // ----------------------------------------------------------------
        void diwec_log_producer_c::set_log_consumer(diwec_log_consumer_i *log_consumer)
        {
            this->log_consumer = log_consumer;
        }
                
        // ----------------------------------------------------------------
        void diwec_log_producer_c::set_log_level(int log_level)
        {
            this->log_level = log_level;
        }
                
        // ----------------------------------------------------------------
        ///
        /// Set log level from string.
        /// If string is not a known log level, then 0 is used, which filters no logging.
        ///
        void diwec_log_producer_c::set_log_level(const char *log_level_label)
        {
            this->log_level = parse_log_level_label(std::string(log_level_label));
        }
                
        // ----------------------------------------------------------------
        ///
        /// Set log level from string.
        /// If string is not a known log level, then 0 is used, which filters no logging.
        ///
        void diwec_log_producer_c::set_log_level(std::string log_level_label)
        {
            this->log_level = parse_log_level_label(log_level_label);
        }
                
        // ----------------------------------------------------------------
        int diwec_log_producer_c::get_log_level()
        {
            return log_level;
        }
                
        // ----------------------------------------------------------------
        diwec_log_consumer_i *diwec_log_producer_c::get_log_consumer()
        {
            return log_consumer;
        }
                

        // ----------------------------------------------------------------
        void diwec_log_producer_c::log(int level, const char *format, ...)
        {
            // printf("........... %s@%d: level is %d, log_level is %d, log_consumer is 0x%08x\n", __FILE__, __LINE__, level, log_level, log_consumer);
            // if (log_consumer == NULL)
            //     printf("........... %s@%d: %s\n", __FILE__, __LINE__, format);
            if (level >= log_level &&
                log_consumer != NULL)
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
                    log_consumer->consume_log(level, buffer);
                    free(buffer);
                }
            }
        }

        // ----------------------------------------------------------------
        void diwec_log_producer_c::log_elapsed(const char *what, int count, double elapsed, const char *warning)
        {
            log(LOG_LEVEL_INFO, "%d %s in %d ms is %d per second %s",
                count,
                what,
                (int)(0.5 + elapsed * 1000.0),
                elapsed == 0.0? 0: (int)(0.5 + count / elapsed),
                warning);
        }

        // ----------------------------------------------------------------
        void diwec_log_producer_c::log_operations(const char *what, int count, double elapsed, const char *warning)
        {
            log(LOG_LEVEL_INFO, "%d %s in %d ms is %0.3f ms per execution %s",
                count,
                what,
                (int)(0.5 + elapsed * 1000.0),
                elapsed * 1000.0 / (double)count,
                warning);
        }

        // ----------------------------------------------------------------
        std::string diwec_log_producer_c::get_log_level_label(int level)
        {
            std::string label;

            switch(level)
            {
                case LOG_LEVEL_CRITICAL:
                    label = std::string("CRITICAL");
                break;
                case LOG_LEVEL_DEBUG:
                    label = std::string("DEBUG");
                break;
                case LOG_LEVEL_ERROR:
                    label = std::string("ERROR");
                break;
                case LOG_LEVEL_INFO:
                    label = std::string("INFO");
                break;
                case LOG_LEVEL_NOTSET:
                    label = std::string("NOTSET");
                break;
                case LOG_LEVEL_WARNING:
                    label = std::string("WARNING");
                break;
                default:
                    label = std::string("level") + std::to_string(level);
            }
            return label; 
        } // end method

        // ----------------------------------------------------------------
        int diwec_log_producer_c::parse_log_level_label(std::string label)
        {
            int level = 0;

            if ((label == "CRITICAL"))
                return LOG_LEVEL_CRITICAL;
            if ((label == "DEBUG"))
                return LOG_LEVEL_DEBUG;
            if ((label == "ERROR"))
                return LOG_LEVEL_ERROR;
            if ((label == "INFO"))
                return LOG_LEVEL_INFO;
            if ((label == "NOTSET"))
                return LOG_LEVEL_NOTSET;
            if ((label == "WARNING"))
                return LOG_LEVEL_WARNING;

            return level; 
        } // end method
    } // end namespace

} // end namespace
