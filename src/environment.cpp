#include <string>
#include <cstring>
#include <sstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sys/time.h>
#include <mutex>
#include <signal.h>
using namespace std;

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>

namespace dtack
{
    namespace base
    {

        extern void throw_runtime_error(const char *format, ...);
        thread_local string thread_name;

        // ------------------------------------------------------------
        environment_c::environment_c()
        {
        } // end constructor

        // ------------------------------------------------------------
        environment_c::~environment_c()
        {

        } // end destructor

        // ----------------------------------------------------------------
        std::string environment_c::get_thread_name()
        {
            if (thread_name == "")
            {
                stringstream ss;
                ss << "T" << std::this_thread::get_id();
                thread_name = ss.str();
            }

            return thread_name;

        } // end destructor

        // ----------------------------------------------------------------
        std::shared_ptr<Incidents> environment_c::get_thread_incidents()
        {
            const char *F = "environment_c::get_thread_incidents";
            if (thread_name == "")
            {
                get_thread_name();
            }


            std::map<string, shared_ptr<Incidents>>::const_iterator found = thread_incidents.find(thread_name);

            // This thread has no incidents list yet?
            if (found == thread_incidents.end())
            {
                std::lock_guard<std::mutex> lock(thread_incidents_mutex);            

                log(LOG_LEVEL_DEBUG, "%s: seeing thread %s for the first time", F, thread_name.c_str());

                // Each thread gets its own incidents assembly.
                shared_ptr<Incidents> incidents = make_shared<Incidents>();
                // log(LOG_LEVEL_INFO, "%s: %s incidents use count initially is %zd", F, thread_name.c_str(), incidents.use_count());
                thread_incidents.insert(std::pair<string, shared_ptr<Incidents>>(thread_name, incidents));
                // log(LOG_LEVEL_INFO, "%s: %s incidents use count after insert is %zd", F, thread_name.c_str(), incidents.use_count());

                //thread_incidents[thread_name] = incidents;

                // Remember the pthread so we can clean up if the thread dies.
                thread_pthreads[thread_name] = pthread_self();
            }

            return thread_incidents[thread_name];
        } // end method

        // ----------------------------------------------------------------
        shared_ptr<Incident> environment_c::append_incident(std::string tag, size_t volume)
        {
            shared_ptr<Incidents> incidents = get_thread_incidents();
            return incidents->append(tag, volume);
        } // end method
                        
        // ----------------------------------------------------------------
        void environment_c::clear_incidents()
        {
            std::lock_guard<std::mutex> lock(thread_incidents_mutex);            
            thread_incidents.clear();
        } // end method

        // ----------------------------------------------------------------
        void environment_c::push_incidents_tag_stack(std::string tag)
        {
            shared_ptr<Incidents> incidents = get_thread_incidents();
            incidents->push_tag_stack(tag);
        } // end method

        // ----------------------------------------------------------------
        void environment_c::push_incidents_tag_stack(const char *tag)
        {
            shared_ptr<Incidents> incidents = get_thread_incidents();
            incidents->push_tag_stack(tag);
        } // end method
                        
        // ----------------------------------------------------------------
        void environment_c::pop_incidents_tag_stack()
        {
            shared_ptr<Incidents> incidents = get_thread_incidents();
            incidents->pop_tag_stack();
        } // end method

        // ----------------------------------------------------------------
        void environment_c::info_incidents_summaries(diwec_log_producer_c *log_producer)
        {
            shared_ptr<Incidents::Summaries> summaries = make_incidents_summaries();
            shared_ptr<Incidents> incidents = get_thread_incidents();
            incidents->info_summaries(summaries.get(), log_producer);
        } // end method

        // ----------------------------------------------------------------
        void environment_c::info_incidents_summaries()
        {
            info_incidents_summaries(this);
        } // end method

        // ----------------------------------------------------------------
        void environment_c::encode_incident_summaries_as_json(std::string& json_string)
        {
            shared_ptr<Incidents::Summaries> summaries = make_incidents_summaries();
            shared_ptr<Incidents> incidents = get_thread_incidents();
            incidents->encode_summaries_as_json(summaries.get(), json_string);

        } // end method

        // ----------------------------------------------------------------
        void merge_summaries(
            std::shared_ptr<Incidents::Summaries> this_summaries,
            std::shared_ptr<Incidents::Summaries> that_summaries)
        {

            // printf(".......... %s@%d: merging to %zd that summaries from %zd this summaries\n", __FILE__, __LINE__, that_summaries->size(), this_summaries->size());

            for (auto this_summary_item: *this_summaries)
            {
                string this_tag = this_summary_item.first;
                std::shared_ptr<Incidents::Summary> this_summary = this_summary_item.second;

                map<std::string, shared_ptr<Incidents::Summary>>::iterator it = that_summaries->find(this_tag);

                if (it == that_summaries->end())
                {
                    std::shared_ptr<Incidents::Summary> that_summary = make_shared<Incidents::Summary>();
                    that_summaries->insert(std::pair<string, shared_ptr<Incidents::Summary>>(this_tag, that_summary));
                    // memcpy(that_summary.get(), this_summary.get(), sizeof(Incidents::Summary));

                    that_summary->accumulation_count = this_summary->accumulation_count;
                    that_summary->accumulation_volume = this_summary->accumulation_volume;
                    that_summary->accumulation_microseconds = this_summary->accumulation_microseconds;

                    that_summary->count = this_summary->count;
                    that_summary->sum = this_summary->sum;
                    that_summary->ave = this_summary->ave;
                    that_summary->min = this_summary->min;
                    that_summary->max = this_summary->max;
                    that_summary->instantaneous_count = this_summary->instantaneous_count;
                    that_summary->instantaneous_volume = this_summary->instantaneous_volume;
                    that_summary->instantaneous_microseconds = this_summary->instantaneous_microseconds;

                    // printf(".......... %s@%d: first time for tag %s count is %zd\n", __FILE__, __LINE__, this_tag.c_str(), that_summary->count);
                }
                else
                {
                    std::shared_ptr<Incidents::Summary> that_summary = that_summaries->at(this_tag);

                    that_summary->accumulation_count += this_summary->accumulation_count;
                    that_summary->accumulation_volume += this_summary->accumulation_volume;
                    // that_summary->accumulation_microseconds = this_summary->accumulation_microseconds;

                    that_summary->count += this_summary->count;
                    that_summary->sum += this_summary->sum;
                    that_summary->ave = (double)that_summary->sum / that_summary->count;
                    if (this_summary->min < that_summary->min) that_summary->min = this_summary->min;
                    if (this_summary->max > that_summary->max) that_summary->max = this_summary->max;
                    that_summary->instantaneous_count += this_summary->instantaneous_count;
                    that_summary->instantaneous_volume += this_summary->instantaneous_volume;

                    // TODO: figure out how to merge the start of the instantanous period
                    // This is only a problem when the same tag is produced in multiple threads.
                    // For now, just use the first merged summary to define the instantaneous period.
                    // We probably need a common time-zero when making summaries for all threads.

                    // struct timeval that_microseconds = that_summary->instantaneous_microseconds;
                    // struct timeval this_microseconds = this_summary->instantaneous_microseconds;
                    // if (timercmp(&this_microseconds, &that_microseconds, <)
                    //     that_summary->instantaneous_microseconds = this_microseconds;

                    // printf(".......... %s@%d: next time for tag %s count is %zd now total %zd\n", __FILE__, __LINE__, this_tag.c_str(), this_summary->count, that_summary->count);
                }
            }

        } // end method

        // ----------------------------------------------------------------
        std::shared_ptr<Incidents::Summaries> environment_c::make_incidents_summaries()
        {
            const char *F = "environment_c::make_incidents_summaries";
            //std::lock_guard<std::mutex> lock(thread_incidents_mutex);            

            shared_ptr<Incidents::Summaries> merged_summaries = make_shared<Incidents::Summaries>();

            for (auto thread_item: thread_incidents)
            {
                string that_thread_name = thread_item.first;

                // log(LOG_LEVEL_INFO, "%s: incidents use count making incident summaries is %zd", F, thread_item.second.use_count());

                if (thread_item.second == NULL)
                {
                    log(LOG_LEVEL_WARNING, "%s: while making incidents summaries, thread_incidents is NULL for %s", F, that_thread_name.c_str());
                    continue;
                }

                if (thread_item.second.use_count() == 0)
                {
                    log(LOG_LEVEL_WARNING, "%s: while making incidents summaries, thread_incidents use_count is 0 for %s", F, that_thread_name.c_str());
                    continue;
                }

                shared_ptr<Incidents::Summaries> thread_summaries = thread_item.second->make_summaries();

                merge_summaries(thread_summaries, merged_summaries);
            }

            return merged_summaries;

        } // end method

        // // ----------------------------------------------------------------
        // void environment_c::prune_incidents_summaries()
        // {
        //     const char *F = "environment_c::make_incidents_summaries";

        //     list<string> dead_threads;

        //     for (auto thread_item: thread_incidents)
        //     {
        //         string thread_id = thread_item.first;

        //         bool is_alive = pthread_kill(thread_pthreads[thread_id], 0) != ESRCH;

        //         if (!is_alive)
        //         {
        //             log(LOG_LEVEL_INFO, "%s: thread 0x%08x looks like it has died", F, thread_id);
        //             dead_threads.push_back(thread_id);
        //         }
        //     }

        //     for (auto thread_id: dead_threads)
        //     {
        //         thread_incidents.erase(thread_id);
        //         thread_pthreads.erase(thread_id);
        //     }

        // } // end method

	} // end namespace

} // end namespace
