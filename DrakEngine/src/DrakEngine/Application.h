#pragma once

#include "DrakEngine/Events/Event.h"
#include "DrakEngine/Events/ApplicationEvent.h"
#include "DrakEngine/Core/Window.h"

namespace DrakEngine {
    class Application {
    public:
        virtual ~Application() = default;

        void Run();
    private:
        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;
    };

    // Must be defined in client
    Application* CreateApplication();
}