#pragma once

#include <thread>
#include <mutex>
#include <pthread.h>

#include <dtack.base.incidents.h>

namespace dtack
{
	namespace base
	{
        class environment_c: public diwec_log_producer_c
        {
        // -----------------------------------------------------------
            public:
                environment_c();
                virtual ~environment_c();

                std::shared_ptr<Incidents> get_thread_incidents();
                std::string get_thread_name();
                void push_incidents_tag_stack(std::string tag);
                void push_incidents_tag_stack(const char *tag);
                void pop_incidents_tag_stack();
                std::shared_ptr<Incident> append_incident(std::string tag, size_t volume=0);
                void clear_incidents();
                std::shared_ptr<Incidents::Summaries> make_incidents_summaries();
                void info_incidents_summaries(diwec_log_producer_c *log_producer);
                void info_incidents_summaries();
                void encode_incident_summaries_as_json(std::string& json_string);

            private:
                std::map<std::string, std::shared_ptr<Incidents>>thread_incidents;
                std::map<std::string, pthread_t> thread_pthreads;
                std::mutex thread_incidents_mutex;

        };

        // -----------------------------------------------------------
        class WithIncident
        {
            public:
                WithIncident(environment_c *environment, std::string tag, size_t volume=0) {incident = environment->append_incident(tag, volume);}
                ~WithIncident() {incident->finalize();}
            public:
                std::shared_ptr<Incident> incident;
        };

        // -----------------------------------------------------------
        class WithIncidentTag
        {
            public:
                WithIncidentTag(environment_c *environment, std::string tag) {this->environment = environment; environment->push_incidents_tag_stack(tag);}
                ~WithIncidentTag() {environment->pop_incidents_tag_stack();}
            private:
                environment_c *environment;
        };

	} // end namespace

} // end namespace
