#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <dtack.base.diwec_logging.h>
#include <dtack.base.environment.h>
#include <dtack.base.limn.h>
#include <dtack.base.dispatcher.h>

namespace dtack
{
	namespace base
	{
		Dispatcher::~Dispatcher() 
		{ 
			// log(diwec_log_producer_c::LOG_LEVEL_DEBUG, "dtack::base::Dispatcher destructor called"); 
		}
	}
}
