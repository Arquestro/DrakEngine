#include "dkpch.h"
#include "DrakEngine/Core/Window.h"

#ifdef DRAK_PLATFORM_MACOS
    #include "Platform/MacOS/MacOSWindow.h"
#endif

namespace DrakEngine {
    Scope<Window> Window::Create(const WindowProps& props)
    {
    #ifdef DRAK_PLATFORM_MACOS
        return CreateScope<MacOSWindow>(props);
    #else
        DRAK_CORE_CRITICAL("Platform not supported or defined!");
        return nullptr;
    #endif
    }
}
