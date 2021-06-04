#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace eon
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#define EON_INFO(...)    ::eon::Log::GetLogger()->info(__VA_ARGS__)
#define EON_WARN(...)    ::eon::Log::GetLogger()->warn(__VA_ARGS__)
#define EON_TRACE(...)   ::eon::Log::GetLogger()->trace(__VA_ARGS__)
#define EON_ERROR(...)   ::eon::Log::GetLogger()->error(__VA_ARGS__)
#define EON_FATAL(...)   ::eon::Log::GetLogger()->critical(__VA_ARGS__)