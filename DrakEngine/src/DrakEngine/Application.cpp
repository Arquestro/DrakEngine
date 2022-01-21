#include "DrakEngine/Application.h"
#include "DrakEngine/Events/MouseEvent.h"
#include "DrakEngine/Events/ApplicationEvent.h"
#include <iostream>

namespace DrakEngine {
    void Application::Run() {
        std::cout << "Start application" << std::endl; 
        auto mme = MouseMovedEvent(0, 0);
        auto ate = AppTickEvent();
    }
}
