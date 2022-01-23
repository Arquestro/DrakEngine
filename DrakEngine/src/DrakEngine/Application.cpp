#include "DrakEngine/Application.h"
#include "DrakEngine/Events/MouseEvent.h"
#include "DrakEngine/Events/ApplicationEvent.h"

namespace DrakEngine {
    void Application::Run() {
        DRAK_CORE_INFO("Start application");
        auto mme = MouseMovedEvent(0, 0);
        auto ate = AppTickEvent();
    }
}
