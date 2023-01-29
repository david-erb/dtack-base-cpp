#pragma once

namespace dtack
{
    namespace base
    {
		namespace dispatcher
		{
			namespace Keywords
			{
				const std::string EVENT = "dtack::base::dispatcher::Keywords::EVENT";
				const std::string COMMAND = "dtack::base::dispatcher::Keywords::COMMAND";
				const std::string STATE = "dtack::base::dispatcher::Keywords::STATE";
				const std::string ERROR = "dtack::base::dispatcher::Keywords::ERROR";
				const std::string CONFIRMATION = "dtack::base::dispatcher::Keywords::CONFIRMATION";
				const std::string INCIDENT_SUMMARIES = "dtack::base::dispatcher::Keywords::INCIDENT_SUMMARIES";
			}

			namespace Events
			{
				const std::string PACKET = "dtack::base::dispatcher::Events::PACKET";
			}

			namespace Commands
			{
				const std::string ARM = "dtack::base::dispatcher::Commands::ARM";
				const std::string STOP = "dtack::base::dispatcher::Commands::STOP";
				const std::string EXIT = "dtack::base::dispatcher::Commands::EXIT";
				const std::string GET_STATE = "dtack::base::dispatcher::Commands::GET_STATE";
				const std::string GET_INCIDENT_SUMMARIES = "dtack::base::dispatcher::Commands::GET_INCIDENT_SUMMARIES";
				const std::string CLEAR_INCIDENTS = "dtack::base::dispatcher::Commands::CLEAR_INCIDENTS";
			}
		}

		// ----------------------------------------------------------------
		class Dispatcher
		{

		public:
			virtual ~Dispatcher() = 0;

			virtual bool dispatch(
				Limn *request,
				Limn *response) = 0;
		};
    }
}

