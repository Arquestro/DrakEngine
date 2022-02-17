#pragma once

#include "DrakEngine/Events/Event.h"
#include "DrakEngine/Events/ApplicationEvent.h"
#include "DrakEngine/Core/Window.h"
#include "DrakEngine/Core/Layer.h"
#include "DrakEngine/Core/LayerStack.h"
#include "DrakEngine/ImGui/ImGuiLayerVulkan.h"

namespace DrakEngine {
    class Application {
    public:
        Application(const std::string& name = "DrakEngine Application");
        virtual ~Application() = default;

        void Run();

        Window& GetWindow() { return *m_Window; }
        static Application& Get() { return *s_Instance; }
    protected:
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
    private:
        void OnEvent(Event& event);
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

        std::unique_ptr<Window> m_Window;
        ImGuiLayerVulkan* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        static Application* s_Instance;
    };

    // Must be defined in client
    Application* CreateApplication();
}