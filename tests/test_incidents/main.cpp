#include <string>
#include <regex>
#include <stdarg.h>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>
#include <sys/time.h>
using namespace std;

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.configuration.h>
#include <dtack.base.limn.h>
#include <dtack.base.cli.h>

#include <dtack.base.incidents.h>

#include <dtack.base.testing.h>

namespace dtack
{
    namespace base
    {
        class PerformanceCalibrator
        {
            public:
                PerformanceCalibrator(int64_t target_microseconds)
                {
                    this->target_microseconds = target_microseconds;
                    gettimeofday(&t0, NULL);
                    actual_count = 0;
                    
                };
                bool next()
                {
                    struct timeval t1;
                    gettimeofday(&t1, NULL);
                    struct timeval dt;
                    timersub(&t1, &t0, &dt);

                    #define US(TV) ((int64_t)(TV)->tv_sec * (int64_t)1000000 + (int64_t)(TV)->tv_usec)
                    actual_microseconds = US(&dt);

                    if (actual_microseconds < target_microseconds)
                    {
                        actual_count++;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                };
                int64_t get_actual_microseconds()
                {
                    return actual_microseconds;
                };
                int64_t get_actual_count()
                {
                    return actual_count;
                };

                private:
                    int64_t target_microseconds;
                    struct timeval t0;
                    int64_t actual_count;
                    int64_t actual_microseconds;
        };
    }
}

using namespace dtack::base;

namespace dtack
{
    namespace base
    {
		// -----------------------------------------------------------

        namespace tests
        {
            namespace incidents
            {
                class Main: public testing::Main
                {
                    public:
                        Main(dtack::base::environment_c *environment);
                        ~Main();
                        virtual int summarize_to_stderr();
                        virtual void run_configured_tests();
                        virtual void test0();
                        virtual void test1();
                        virtual void test2();
                        virtual void test2_thread();
                        virtual void test3();
                        virtual void test3_thread();
                        virtual void test4();
                        virtual void test4_thread1();
                        virtual void test4_thread2();
                        virtual void test4_thread3();
                        virtual void assert_thread_incidents_use_counts(size_t count);
                        virtual void assert_incident_use_counts(size_t count);
                        virtual void nop_particle();
                        virtual void incident_particle();

                    private:
                        vector<std::shared_ptr<Incidents>> thread_incidents_list;
                        vector<std::shared_ptr<Incident>> incident_list;

                        int64_t particle_count;

                        int64_t test4_microseconds;
                }; // end class

                // ----------------------------------------------------------------
                Main::Main(dtack::base::environment_c *environment):
                    testing::Main(environment)
                {

                } // end method

                // ----------------------------------------------------------------
                int Main::summarize_to_stderr()
                {
                    DTACK_COMMON_FUNCTION;

                    return testing::Main::summarize_to_stderr(F);
                } // end method
                
                // ----------------------------------------------------------------
                Main::~Main()
                {

                } // end method

                // ----------------------------------------------------------------
                void Main::run_configured_tests()
                {
                    ///
                    /// This runs the just one test: test4().
                    /// The test::Main base class assert methods don't return an error or raise an exception.
                    /// Instead they just increment the instance variables pass_count and fail_count.
                    /// If fail_count is left at 0 when the function returns, the whole test will fail.
                    ///

                    DTACK_COMMON_FUNCTION;

                    // Set ourselves as the log consumer.
                    set_log_consumer(this);

                    // Set the level at which we will send log entries to the consumer.
                    set_log_level(configuration->peek_log_level());

                    const char *doing = "running";
                    try
                    {
                        // doing = "test0";
                        // test0();
                        // doing = "test1";
                        // test1();
                        // doing = "test2";
                        // test2();
                        // doing = "test3";
                        // test3();
                        doing = "test4";
                        test4();
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
                void Main::assert_thread_incidents_use_counts(size_t count)
                {
                    DTACK_COMMON_FUNCTION;

                    assert_equal(F, "thread incidents count", count, thread_incidents_list.size());

                    for(size_t i=0; i<thread_incidents_list.size(); i++)
                    {
                        string assertion = printf_to_string("thread_incidents_list %zd has been fully released", i);
                        assert_equal(F, assertion.c_str(), thread_incidents_list[i].use_count(), 1);
                    }

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::assert_incident_use_counts(size_t count)
                {
                    DTACK_COMMON_FUNCTION;

                    assert_equal(F, "incident count", count, incident_list.size());

                    for(size_t i=0; i<incident_list.size(); i++)
                    {
                        string assertion = printf_to_string("incident %zd has been fully released", i);
                        assert_equal(F, assertion.c_str(), incident_list[i].use_count(), 1);
                    }

                } // end method

                // ----------------------------------------------------------------
                void Main::test0()
                {
                    DTACK_COMMON_FUNCTION;

                    {
                        WithIncident with(environment, "x");
                        with.incident->volume = 2;
                        incident_list.push_back(with.incident);
                    }

                    {
                        WithIncident with(environment, "x");
                        with.incident->volume = 2;
                        incident_list.push_back(with.incident);
                    }

                    environment->info_incidents_summaries(this);

                    environment->clear_incidents();

                    assert_incident_use_counts(2);
                    incident_list.clear();

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test1()
                {
                    DTACK_COMMON_FUNCTION;

                    environment->push_incidents_tag_stack("a");

                    {
                        std::shared_ptr<Incident> incident = environment->append_incident("x");
    					std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        incident->finalize();
                        incident_list.push_back(incident);
                    }

                    environment->push_incidents_tag_stack("b");
                    {
                        WithIncident with(environment, "x");
    					std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        incident_list.push_back(with.incident);
                    }

                    environment->info_incidents_summaries(this);

                    environment->clear_incidents();
                    
                    assert_incident_use_counts(2);
                    incident_list.clear();

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test2()
                {
                    DTACK_COMMON_FUNCTION;

					std::thread *thread1 = new std::thread(&Main::test2_thread, this);
					std::thread *thread2 = new std::thread(&Main::test2_thread, this);
					std::thread *thread3 = new std::thread(&Main::test2_thread, this);

					thread1->join();
					thread2->join();
					thread3->join();

                    environment->info_incidents_summaries(this);

                    environment->clear_incidents();

                    // Check the per-thread incidents got released.
                    assert_thread_incidents_use_counts(3);
                    // Check the incidents themselves got released.
                    thread_incidents_list.clear();

                    assert_incident_use_counts(6);
                    incident_list.clear();

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test2_thread()
                {
                    DTACK_COMMON_FUNCTION;

                    thread_incidents_list.push_back(environment->get_thread_incidents());

                    environment->push_incidents_tag_stack("test2_thread");

                    {
                        WithIncident with(environment, printf_to_string("0x%08x", std::this_thread::get_id()));
    					std::this_thread::sleep_for(std::chrono::milliseconds(5));
                        incident_list.push_back(with.incident);
                    }

                    {
                        WithIncident with(environment, "all");
    					std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        incident_list.push_back(with.incident);
                    }

                    // log(LOG_LEVEL_INFO, "thread 0x%08x is done", std::this_thread::get_id());

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test3()
                {
                    DTACK_COMMON_FUNCTION;

					std::thread *thread1 = new std::thread(&Main::test3_thread, this);
					std::thread *thread2 = new std::thread(&Main::test3_thread, this);
					std::thread *thread3 = new std::thread(&Main::test3_thread, this);

					thread1->join();
					thread2->join();
					thread3->join();

                    environment->info_incidents_summaries(this);

                    environment->clear_incidents();

                    // Check the per-thread incidents got released.
                    assert_thread_incidents_use_counts(3);
                    // Check the incidents themselves got released.
                    thread_incidents_list.clear();

                    assert_incident_use_counts(0);
                    incident_list.clear();

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test3_thread()
                {
                    DTACK_COMMON_FUNCTION;

                    thread_incidents_list.push_back(environment->get_thread_incidents());

                    environment->push_incidents_tag_stack("test3_thread");

                    for(size_t i=0; i<dtack::base::IncidentAccumulation::DEFAULT_CIRCULAR_BUFFER_SIZE+3; i++)
                    {
                        WithIncident with1(environment, printf_to_string("0x%08x first", std::this_thread::get_id()));
                        with1.incident->add_volume(1);
                        WithIncident with2(environment, "all");
                    }
                    for(size_t i=0; i<dtack::base::IncidentAccumulation::DEFAULT_CIRCULAR_BUFFER_SIZE+3; i++)
                    {
                        WithIncident with1(environment, printf_to_string("0x%08x second", std::this_thread::get_id()));
                        with1.incident->add_volume(1);
                        WithIncident with2(environment, "all");
                    }

                } // end method
                
                // ----------------------------------------------------------------
                void Main::test4()
                {
                    DTACK_COMMON_FUNCTION;

                    test4_microseconds = 10000000;

#if 0
                    // Run the performance calibrator with debug.
                    {
                        PerformanceCalibrator calibrator(test4_microseconds);
                        particle_count = 0;
                        while(calibrator.next())
                        {
                            nop_particle();
                        }
                        log(LOG_LEVEL_DEBUG, "%s: nop_calibration_count is %zd, particle count is %zd", F, 
                            calibrator.get_actual_count(), particle_count);
                    }
#endif

					std::thread *thread1a = NULL;
					std::thread *thread1b = NULL;
					std::thread *thread2 = NULL;
					std::thread *thread3 = NULL;
                    
                    // Fire up all the threads.
					thread1a = new std::thread(&Main::test4_thread1, this);
					thread1b = new std::thread(&Main::test4_thread1, this);
					thread2 = new std::thread(&Main::test4_thread2, this);
					thread3 = new std::thread(&Main::test4_thread3, this);

                    // Wait for all the threads to finish.
					if (thread1a) thread1a->join(); 
					if (thread1b) thread1b->join();
					if (thread2) thread2->join();
					if (thread3) thread3->join();

                    // Emit the incident summary to the info stream.
                    environment->info_incidents_summaries(this);

                    // Clear the incidents.
                    environment->clear_incidents();

                    // Check the per-thread incidents got released.
                    assert_thread_incidents_use_counts(4);
                    // Check the incidents themselves got released.
                    thread_incidents_list.clear();

                    // Check the use counts all got released.
                    assert_incident_use_counts(0);
                    incident_list.clear();

                } // end method
                    
                // ----------------------------------------------------------------
                void Main::nop_particle()
                {
                    particle_count++;
                } // end method
                    
                // ----------------------------------------------------------------
                void Main::incident_particle()
                {
                    particle_count++;
                } // end method
                    
                // ----------------------------------------------------------------
                void Main::test4_thread1()
                {
                    DTACK_COMMON_FUNCTION;

                    thread_incidents_list.push_back(environment->get_thread_incidents());

                    // Create tag to mark that all incidents are coming from this thread.
                    environment->push_incidents_tag_stack("test4_thread1");

                    string incident_tag = environment->get_thread_name();

                    // Generate some incidents.
                    {
                        PerformanceCalibrator calibrator(test4_microseconds);
                        while(calibrator.next())
                        {
                            WithIncident with(environment, incident_tag);
                            with.incident->add_volume(1);
                        }
                        log(LOG_LEVEL_DEBUG, "%s: incident count is %zd", F, 
                            calibrator.get_actual_count());
                    }
    
                } // end method

                // ----------------------------------------------------------------
                void Main::test4_thread2()
                {
                    DTACK_COMMON_FUNCTION;

                    thread_incidents_list.push_back(environment->get_thread_incidents());

                    // Create tag to mark that all incidents are coming from this thread.
                    environment->push_incidents_tag_stack("test4_thread2");

                    string incident_tag = environment->get_thread_name();

                    // Generate some incidents.
                    {
                        PerformanceCalibrator calibrator(test4_microseconds);
                        while(calibrator.next())
                        {
                            WithIncident with(environment, incident_tag);
                            with.incident->add_volume(1);
                        }
                        log(LOG_LEVEL_DEBUG, "%s: incident count is %zd", F, 
                            calibrator.get_actual_count());
                    }
    
                } // end method

                // ----------------------------------------------------------------
                void Main::test4_thread3()
                {
                    DTACK_COMMON_FUNCTION;

                    // Create tag to mark that all incidents are coming from this thread.
                    thread_incidents_list.push_back(environment->get_thread_incidents());

                    environment->push_incidents_tag_stack("test4_thread2");

                    string incident_tag = environment->get_thread_name();
            
                    // Generate some incidents.
                    {
                        PerformanceCalibrator calibrator(test4_microseconds);
                        while(calibrator.next())
                        {
                            WithIncident with(environment, incident_tag);
                            std::shared_ptr<Incidents::Summaries> summaries = environment->make_incidents_summaries();
                            // string json;
                            // environment->encode_incident_summaries_as_json(json);
                            with.incident->add_volume(1);
                        }
                        log(LOG_LEVEL_DEBUG, "%s: ecoding count is %zd", F, 
                            calibrator.get_actual_count());
                    }
    
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
    dtack::base::tests::incidents::Main main(&environment);

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
