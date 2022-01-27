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

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(DRAK_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DRAK_BIND_EVENT_FN(Application::OnWindowResize));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        if(e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        return false;
    }
}
