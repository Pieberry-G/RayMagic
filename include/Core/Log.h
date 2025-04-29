#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace RayMagic {

	class __declspec(dllexport) Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger();
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core log macros
#define RM_CORE_TRACE(...)	::RayMagic::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RM_CORE_INFO(...)	::RayMagic::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RM_CORE_WARN(...)	::RayMagic::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RM_CORE_ERROR(...)	::RayMagic::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RM_CORE_FATAL(...)	::RayMagic::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define RM_TRACE(...)		::RayMagic::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RM_INFO(...)		::RayMagic::Log::GetClientLogger()->info(__VA_ARGS__)
#define RM_WARN(...)		::RayMagic::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RM_ERROR(...)		::RayMagic::Log::GetClientLogger()->error(__VA_ARGS__)
#define RM_FATAL(...)		::RayMagic::Log::GetClientLogger()->fatal(__VA_ARGS__)