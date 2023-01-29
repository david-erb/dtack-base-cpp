#pragma once

#include <vector>
#include <list>
#include <map>
#include <mutex>
#include <memory>

#include <dtack.base.circular_buffer.h>

namespace dtack
{
    namespace base
    {
        class IncidentAccumulation;

        class Incident
        {
            public:
                Incident(IncidentAccumulation *accumulation, std::string tag, size_t volume=0);
                ~Incident();
                
                void add_volume(size_t volume);
                void finalize();

                struct timeval start_timeval;
                struct timeval final_timeval;
                size_t volume;
                IncidentAccumulation *accumulation;
        }; // end class

        // ----------------------------------------------------------------
        class IncidentAccumulation
        {
            public:
                IncidentAccumulation(std::string tag);
                ~IncidentAccumulation();
                std::shared_ptr<Incident> append(size_t volume);

                static const size_t DEFAULT_CIRCULAR_BUFFER_SIZE = 10000;

            public:
                std::string tag;
                struct timeval start_timeval;
                size_t count;
                size_t volume;
                CircularBuffer<std::shared_ptr<Incident>> *circular_buffer;
        };


        // ----------------------------------------------------------------
        class Incidents
        {
            public:
                typedef struct 
                {
                    size_t accumulation_count;
                    size_t accumulation_volume;
                    size_t accumulation_microseconds;
                    size_t count; 
                    size_t sum; 
                    size_t ave; 
                    size_t min; 
                    size_t max;
                    size_t instantaneous_microseconds;
                    size_t instantaneous_count;
                    size_t instantaneous_volume;
                } Summary;

                typedef std::map<std::string, std::shared_ptr<Summary>> Summaries;
            public:
                Incidents();
                ~Incidents();
                void push_tag_stack(std::string tag);
                void push_tag_stack(const char *tag);
                void pop_tag_stack();
                std::shared_ptr<Incident> append(std::string tag, size_t volume=0);
                void append(Incidents *incidents);

                void info_summaries(Summaries *summaries, diwec_log_producer_c *log_producer);
                void format_summaries(Summaries *summaries, std::vector<std::string>& lines);
                void encode_summaries_as_json(Summaries *summaries, std::string& json_string);
                std::shared_ptr<Summaries> make_summaries();
                std::shared_ptr<Summary> make_summary(std::shared_ptr<IncidentAccumulation> incident_accumulation);
            
            private:
                void prune();

                const size_t MAX_TAG_STACK_LENGTH = 1024;
                char *tag_stack;
                std::map<std::string, std::shared_ptr<IncidentAccumulation>> incident_accumulations;

        }; // end interface

        // ----------------------------------------------------------------
        Incidents *new_Incidents();

    } // end namespace

} // end namespace
