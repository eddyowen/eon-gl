#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace eon
{
	std::shared_ptr<spdlog::logger> Log::s_logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger = spdlog::stdout_color_mt("APP");
		s_logger->set_level(spdlog::level::trace);
	}
}