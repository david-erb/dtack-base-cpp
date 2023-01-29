#include <sys/time.h>
#include <string>
#include <regex>
#include <stdarg.h>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.base.h>
#include <dtack.base.limn.h>
using namespace dtack::base;

namespace dtack
{
    namespace base
    {
        // ----------------------------------------------------------------
        Incident::Incident(IncidentAccumulation *accumulation, string tag, size_t volume)
        {
            // const char* F = "Incident";
            // printf(".......... %s: constructing for tag %s\n", F, tag.c_str());

            this->accumulation = accumulation;
            this->volume = volume;
			gettimeofday(&start_timeval, NULL);
            timerclear(&final_timeval);
        }
        
        // ----------------------------------------------------------------
        Incident::~Incident()
        {
            // const char* F = "~Incident";
            // printf(".......... %s: destructing for tag %s\n", F, tag.c_str());
        }

        // ----------------------------------------------------------------
        void Incident::add_volume(size_t volume)
        {
			this->volume += volume;
            this->accumulation->volume += volume;
        }

        // ----------------------------------------------------------------
        void Incident::finalize()
        {
			gettimeofday(&final_timeval, NULL);
        }

    } // end namespace

} // end namespace