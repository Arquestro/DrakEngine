#include "DrakEngine.h"
#include "DrakEngine/Entrypoint.h"

class Sandbox : public DrakEngine::Application {
public:
    ~Sandbox()
    {

    }
};

DrakEngine::Application* DrakEngine::CreateApplication() {
    return new Sandbox();
}
