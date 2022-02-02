#include <utility>

#include "DrakEngine.h"
#include "DrakEngine/Entrypoint.h"


class ExampleLayer : public DrakEngine::Layer {
public:
    ExampleLayer() : Layer("example") {}

    void OnUpdate() override {
        DRAK_INFO("ExampleLayer::Update");
    }
    void OnEvent(DrakEngine::Event &event) override {
        DRAK_TRACE("{0}", event);
    }
};

class Sandbox : public DrakEngine::Application {
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

DrakEngine::Application* DrakEngine::CreateApplication() {
    return new Sandbox();
}
