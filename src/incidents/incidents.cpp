#include <stdio.h>
#include <cstring>
#include <string>
#include <regex>
#include <stdarg.h>
#include <sstream>
#include <sys/time.h>
#include <vector>
#include <unordered_map>
using namespace std;

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.incidents.h>


#include <dtack.base.string_token.h>
#include <dtack.base.string_walker.h>
#include <dtack.base.packet.h>

using namespace dtack::base;

namespace dtack
{
    namespace base
    {
       
        // ----------------------------------------------------------------
        IncidentAccumulation::IncidentAccumulation(std::string tag) 
        {
            // const char* F = "IncidentAccumulation";
            // printf(".......... %s: constructing for tag %s\n", F, tag.c_str());
            this->tag = tag;
            this->count = 0;
            this->volume = 0;
			gettimeofday(&start_timeval, NULL);
            circular_buffer = new CircularBuffer<std::shared_ptr<Incident>>(DEFAULT_CIRCULAR_BUFFER_SIZE);
        }
        IncidentAccumulation::~IncidentAccumulation() 
        {
            // const char* F = "~IncidentAccumulation";
            // printf(".......... %s: destructing for tag %s\n", F, tag.c_str());
            if (circular_buffer != NULL)
                delete circular_buffer;
        }
        std::shared_ptr<Incident> IncidentAccumulation::append(size_t volume)
        {
            this->volume += volume;
            this->count++;
            std::shared_ptr<Incident> incident = std::make_shared<Incident>(this, tag, volume);
            circular_buffer->push(incident);
            return incident;
        }


        // ----------------------------------------------------------------
        Incidents *new_Incidents()
        {
            return new Incidents();
        }

        // ----------------------------------------------------------------
        Incidents::Incidents():
            tag_stack(NULL)
        {
            // const char* F = "Incidents";
            // printf(".......... %s: constructing\n", F);
            tag_stack = (char *)allocate_from_heap("Incidents", "tag stack", MAX_TAG_STACK_LENGTH, 1);
            *tag_stack = '\0';
        }
        
        // ----------------------------------------------------------------
        Incidents::~Incidents()
        {
            // const char* F = "~Incidents";
            // printf(".......... %s: destructing\n", F);
            if (tag_stack != NULL)
            {
                free(tag_stack);
                tag_stack = NULL;
            }
        }
        
        // ----------------------------------------------------------------
        shared_ptr<Incident> Incidents::append(string tag, size_t volume)
        {
            string full_tag = tag_stack + tag;

            map<string, shared_ptr<IncidentAccumulation>>::iterator it = incident_accumulations.find(full_tag);

            shared_ptr<IncidentAccumulation> incident_accumulation;
            if (it == incident_accumulations.end())
            {
                incident_accumulation = make_shared<IncidentAccumulation>(full_tag);
                incident_accumulations[full_tag] = incident_accumulation;
            }
            else
            {
                incident_accumulation = incident_accumulations[full_tag];
            }

            return incident_accumulation->append(volume);
        } // end method

        // ----------------------------------------------------------------
        void Incidents::push_tag_stack(string tag)
        {
            push_tag_stack(tag.c_str());
        } // end method

        // ----------------------------------------------------------------
        // Pushes a tag on the stack.  This is used to format the incident names.
        // The names of the tags will all be appended to the actual primary incident name.
        // It is meant to be used when entering a processing section.
        void Incidents::push_tag_stack(const char *tag)
        {
            size_t length = strlen(tag_stack);
            if (length + strlen(tag) + 1> MAX_TAG_STACK_LENGTH)
            {
                string message = printf_to_string("tag stack maximum length %zd exceeded at tag %s", MAX_TAG_STACK_LENGTH, tag);
                throw runtime_error(message.c_str());
            }
            strcat(tag_stack, tag);
            strcat(tag_stack, "/");
        } // end method
                        
        // ----------------------------------------------------------------
        void Incidents::pop_tag_stack()
        {
            if (strlen(tag_stack) > 0)
                tag_stack[strlen(tag_stack)-1] = '\0';
            char *p = strrchr(tag_stack, '/');
            if (p == NULL)
            {
                p = tag_stack;
            }
            else
            {
                *(p+1) = '\0';
            }

        } // end method

        // ----------------------------------------------------------------
        void Incidents::info_summaries(Summaries *summaries, diwec_log_producer_c *log_producer)
        {
            vector<string> lines;
            format_summaries(summaries, lines);

            for (vector<string>::iterator line = lines.begin(); line != lines.end(); line++)
                log_producer->log(diwec_log_producer_c::LOG_LEVEL_INFO, "%s", line->c_str());

        } // end method

        // ----------------------------------------------------------------
        void Incidents::format_summaries(Summaries *summaries, vector<string>& lines)
        {
            lines.clear();

            lines.push_back(printf_to_string("%80s       %-30s |  %s",    
                "",
                "overall",
                "buffered millisesconds"));

            lines.push_back(printf_to_string("%80s  %8s  %11s  %11s  |  %5s  %5s  %5s  %5s  %5s",    
                "tag",
                "count",
                "volume",
                "microsec",
                "count",
                "sum",
                "ave",
                "min",
                "max"));

            lines.push_back(printf_to_string("%80s  %8s  %11s  %11s  |  %5s  %5s  %5s  %5s  %5s",
                "---",
                "--------",
                "-----------",
                "-----------",
                "-----",
                "---",
                "---",
                "---",
                "---"));

            for (auto summary_item: *summaries)
            {
                shared_ptr<Summary> summary = summary_item.second;
                lines.push_back(printf_to_string("%80s  %8zd  %11zd  %11zd  |  %5zd  %5zd  %5zd  %5zd  %5zd",
                    summary_item.first.c_str(),

                    summary->accumulation_count,
                    summary->accumulation_volume,
                    summary->accumulation_microseconds,
                    
                    summary->count,
                    summary->sum,
                    summary->ave,
                    summary->min,
                    summary->max));
            }
        } // end method

        // ----------------------------------------------------------------
        void Incidents::encode_summaries_as_json(Summaries *summaries, std::string& json_string)
        {
            environment_c *environment = new environment_c();
            std::shared_ptr<packet_c> packet = std::make_shared<packet_c>(environment);
            for (auto summary_item: *summaries)
            {
                shared_ptr<Summary> summary = summary_item.second;
                std::shared_ptr<packet_c> sub_packet = packet->new_packet(summary_item.first);


                sub_packet->add_scalar("accumulation_count", 
                    to_string(summary->accumulation_count));
                sub_packet->add_scalar("accumulation_volume", 
                    to_string(summary->accumulation_volume));
                sub_packet->add_scalar("accumulation_microseconds", 
                    to_string(summary->accumulation_microseconds));

                sub_packet->add_scalar("count", 
                    to_string(summary->count));
                sub_packet->add_scalar("sum", 
                    to_string(summary->sum));
                sub_packet->add_scalar("ave", 
                    to_string(summary->ave));
                sub_packet->add_scalar("min", 
                    to_string(summary->min));
                sub_packet->add_scalar("max", 
                    to_string(summary->max));
                sub_packet->add_scalar("instantaneous_microseconds", 
                    to_string(summary->instantaneous_microseconds));
                sub_packet->add_scalar("instantaneous_count", 
                    to_string(summary->instantaneous_count));
                sub_packet->add_scalar("instantaneous_volume", 
                    to_string(summary->instantaneous_volume));
            }

            packet->compose_jso_string(json_string);

        } // end method

        // ----------------------------------------------------------------
        std::shared_ptr<Incidents::Summaries> Incidents::make_summaries()
        {
            DTACK_COMMON_FUNCTION;

            shared_ptr<Summaries> summaries = std::make_shared<Summaries>();

            for (auto incident_accumulation_item: incident_accumulations)
            {
                shared_ptr<IncidentAccumulation> incident_accumulation = incident_accumulation_item.second;

                if (incident_accumulation == NULL)
                {
                    printf(".......... %s: while making incident summary, incident_accumulation is NULL for %s", F, incident_accumulation_item.first.c_str());
                    continue;
                }

                shared_ptr<Summary> summary = make_summary(incident_accumulation);

                // If make_summary returns NULL for a tag, it means no completed incidents for that tag.
                if (summary != NULL)
                {
                    string tag = incident_accumulation->tag;

                    (*summaries)[tag] = summary;
                }
            }

            return summaries;

        } // end method


        // ----------------------------------------------------------------
        std::shared_ptr<Incidents::Summary> Incidents::make_summary(shared_ptr<IncidentAccumulation> incident_accumulation)
        {
            size_t count = 0;
            struct timeval sum_delta_timeval; timerclear(&sum_delta_timeval);
            struct timeval min_delta_timeval; timerclear(&min_delta_timeval);
            struct timeval max_delta_timeval; timerclear(&max_delta_timeval);

            struct timeval now_timeval; gettimeofday(&now_timeval, NULL);
            struct timeval instantaneous_start_timeval; timerclear(&instantaneous_start_timeval);
            struct timeval accumulation_final_timeval; timerclear(&accumulation_final_timeval);
            size_t instantaneous_count = 0;
            size_t instantaneous_volume = 0;
            size_t instantaneous_definition_microseconds = 5000 * 1000;

            #define MS(TV) ((TV)->tv_sec * 1000 + (TV)->tv_usec / 1000)
            #define US(TV) ((TV)->tv_sec * 1000000 + (TV)->tv_usec)

// printf(".......... %s@%d: initial use_count is %zd\n", __FILE__, __LINE__, incident_pointer.use_count());
            for(size_t i=0; true; i++)
            {
                shared_ptr<Incident> incident_pointer;
                if (!incident_accumulation->circular_buffer->read(i, incident_pointer))
                {
                    break;
                }
// if (incident_pointer.use_count() == 0)
// printf(".......... %s@%d: %zd use count %zd\n", __FILE__, __LINE__, i, incident_pointer.use_count());

                Incident *incident = incident_pointer.get();
                if (incident == NULL)
                    throw_runtime_error("incident read from circular buffer is NULL at index %zd", i);
                    
                if (timerisset(&incident->final_timeval))
                {
                    struct timeval delta_timeval;
                    timersub(&incident->final_timeval, &incident->start_timeval, &delta_timeval);

                    timeradd(&sum_delta_timeval, &delta_timeval, &sum_delta_timeval);

                    if (!timerisset(&min_delta_timeval) ||
                        timercmp(&delta_timeval, &min_delta_timeval, <))
                        min_delta_timeval = delta_timeval;

                    if (!timerisset(&max_delta_timeval) ||
                        timercmp(&delta_timeval, &max_delta_timeval, >))
                        max_delta_timeval = delta_timeval;

                    count = count + 1;

                    // Age of this incident.
                    struct timeval incident_age_timeval;
                    timersub(&now_timeval, &incident->start_timeval, &incident_age_timeval);

                    // Age in micro for easy thresholding.
                    size_t age_microseconds = US(&incident_age_timeval);

                    // Age is within the defined instantaneous window?
                    if (age_microseconds <= instantaneous_definition_microseconds)
                    {
                        // Count incidents within the instantaneous window.
                        instantaneous_count++;

                        // Sum volume of all incidents within the instantaneous window.
                        instantaneous_volume += incident->volume;

                        // Keep the earliest incident within the instantaneous window.
                        if (!timerisset(&instantaneous_start_timeval) ||
                             timercmp(&incident->start_timeval, &instantaneous_start_timeval, <))
                            instantaneous_start_timeval = incident->start_timeval;
                    }

                    // Keep the latest incident within the accumulation window.
                    if (!timerisset(&accumulation_final_timeval) ||
                            timercmp(&incident->final_timeval, &accumulation_final_timeval, >))
                        accumulation_final_timeval = incident->final_timeval;

                }
            }
//printf(".......... %s@%d: final use_count is %zd\n", __FILE__, __LINE__, incident_pointer.use_count());

            if (count > 0)
            {
                shared_ptr<Summary> summary = make_shared<Summary>();

                summary->accumulation_count = incident_accumulation->count;
                summary->accumulation_volume = incident_accumulation->volume;
                struct timeval period_timeval;
                timersub(&accumulation_final_timeval, &incident_accumulation->start_timeval, &period_timeval);
                summary->accumulation_microseconds = US(&period_timeval);

                summary->count = count;
                summary->sum = MS(&sum_delta_timeval);
                summary->ave = int(MS(&sum_delta_timeval) / (count == 0? 1.0: double(count)));
                summary->min = MS(&min_delta_timeval);
                summary->max = MS(&max_delta_timeval);

                if (instantaneous_count)
                {
                    struct timeval instantaneous_microseconds;
                    timersub(&now_timeval, &instantaneous_start_timeval, &instantaneous_microseconds);

                    summary->instantaneous_microseconds = US(&instantaneous_microseconds);
                    summary->instantaneous_count = instantaneous_count;
                    summary->instantaneous_volume = instantaneous_volume;
                }
                else
                {
                    summary->instantaneous_microseconds = 0;
                    summary->instantaneous_count = 0;
                    summary->instantaneous_volume = 0;
                }
                
                
                return summary;
            }
            else
            {
                return NULL;
            }
            
        } // end method

    } // end namespace

} // end namespace