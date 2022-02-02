#pragma once

#include "DrakEngine/Events/Event.h"
#include "DrakEngine/Events/ApplicationEvent.h"
#include "DrakEngine/Core/Window.h"
#include "DrakEngine/Core/Layer.h"
#include "DrakEngine/Core/LayerStack.h"

namespace DrakEngine {
    class Application {
    public:
        virtual ~Application() = default;

        void Run();
    protected:
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
    private:
        void OnEvent(Event& event);
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
    };

    // Must be defined in client
    Application* CreateApplication();
}