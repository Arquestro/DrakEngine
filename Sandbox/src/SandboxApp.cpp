#include <utility>
#include "DrakEngine.h"

class Sandbox : public DrakEngine::Application {
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }
};

DrakEngine::Application* DrakEngine::CreateApplication() {
    return new Sandbox();
}
