#pragma once 

#include "DrakEngine/Core/Base.h"

// This ignores all warning raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace DrakEngine {
    class Logger {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define DRAK_CORE_TRACE(...)    ::DrakEngine::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define DRAK_CORE_INFO(...)     ::DrakEngine::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define DRAK_CORE_WARN(...)     ::DrakEngine::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define DRAK_CORE_ERROR(...)    ::DrakEngine::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define DRAK_CORE_CRITICAL(...) ::DrakEngine::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DRAK_TRACE(...)         ::DrakEngine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define DRAK_INFO(...)          ::DrakEngine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define DRAK_WARN(...)          ::DrakEngine::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define DRAK_ERROR(...)         ::DrakEngine::Logger::GetClientLogger()->error(__VA_ARGS__)
#define DRAK_CRITICAL(...)      ::DrakEngine::Logger::GetClientLogger()->critical(__VA_ARGS__)
