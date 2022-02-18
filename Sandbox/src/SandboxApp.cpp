#include "DrakEngine.h"
#include "DrakEngine/Entrypoint.h"

class ImGuiExampleLayer : public DrakEngine::Layer {
public:
    ImGuiExampleLayer(const std::string& name) : DrakEngine::Layer(name) {}
    void OnImGuiLayer() override {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
};

class Sandbox : public DrakEngine::Application {
public:
    Sandbox(const std::string& name) : DrakEngine::Application(name) {
        PushLayer(new ImGuiExampleLayer("ImGuiExampleLayer"));
    }

    ~Sandbox() {}
};

DrakEngine::Application* DrakEngine::CreateApplication() {
    return new Sandbox("Sandbox Application");
}
