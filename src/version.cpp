#include <chrono>
#include <cstring>
#include <stdarg.h>
#include <stdexcept>
#include <unordered_map>
using namespace std;

#include "dtack.base.diwec_logging.h"
#include <dtack.base.environment.h>
#include <dtack.base.base.h>

#define VERSION "4.3.2"

namespace dtack
{
    namespace base
    {
        // ------------------------------------------------------------
        Version version()
        {
            DTACK_COMMON_FUNCTION;

            Version version;
            version.version = VERSION;
            version.module = F;
            version.module = version.module.substr(0, version.module.rfind("::"));
            version.compile_date_string = format_compiled_date(__DATE__, __TIME__);

            return version;
        } // end method

    } // end namespace

} // end namespace
