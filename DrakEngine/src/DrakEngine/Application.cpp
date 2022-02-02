#include "DrakEngine/Core/Window.h"
#include "DrakEngine/Application.h"
#include "DrakEngine/Events/ApplicationEvent.h"

namespace DrakEngine {
    void Application::Run() {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(DRAK_BIND_EVENT_FN(Application::OnEvent));
        while(m_Running) {
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
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }
}
