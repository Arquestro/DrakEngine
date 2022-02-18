#include "DrakEngine/Core/Window.h"
#include "DrakEngine/Application.h"
#include "DrakEngine/Events/ApplicationEvent.h"

namespace DrakEngine {
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        s_Instance = this;
        m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(DRAK_BIND_EVENT_FN(Application::OnEvent));
        m_ImGuiLayer = static_cast<ImGuiLayer*>(new ImGuiLayerVulkan());
        PushOverlay(m_ImGuiLayer);
    }

    void Application::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                for (Layer* layer : m_LayerStack) {
                    layer->OnUpdate();
                }
                m_ImGuiLayer->OnBegin();
                for (Layer* layer : m_LayerStack) {
                    layer->OnImGuiLayer();
                }
                m_ImGuiLayer->OnEnd();
            }
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(DRAK_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DRAK_BIND_EVENT_FN(Application::OnWindowResize));
        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
            (*--it)->OnEvent(event);
            if(event.Handled) break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& event) {
        if(event.GetWidth() == 0 || event.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        return false;
    }

    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *overlay) {
        m_LayerStack.PushOverlay(overlay);
    }
}
