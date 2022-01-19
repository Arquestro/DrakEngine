#pragma once

namespace DrakEngine {
    class Application {
    public:
        Application() {};
        ~Application() {};
        void Run();
    };

    // Must be defined in client
    Application* CreateApplication();
}