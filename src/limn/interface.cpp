#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

#include "dtack.base.diwec_logging.h"
#include "dtack.base.environment.h"
#include "dtack.base.base.h"
#include "dtack.base.limn.h"

namespace dtack
{
    namespace base
    {
        // ----------------------------------------------------------------
        // This is the abstract interface destructor.
        Limn::~Limn()
        {
            // printf("~Limn\n");
        } // end destructor

        // ----------------------------------------------------------------
        // These are the externs to the implementation factories.
        // There is only this one for now.
        namespace limn {namespace packets {dtack::base::Limn *new_Limn(environment_c *environment);}}

        // ----------------------------------------------------------------
        Limn *new_Limn(environment_c *environment, const char *class_name)
        {
            Limn *limn;
            limn = limn::packets::new_Limn(environment);
            return limn;
        }; // end function

        Limn *new_Limn(environment_c *environment, std::string class_name)
        {
            return new_Limn(environment, class_name.c_str());
        }; // end function

        Limn *new_Limn(environment_c *environment)
        {
            return new_Limn(environment, NULL);
        }; // end function

    } // end namespace

} // end namespace
