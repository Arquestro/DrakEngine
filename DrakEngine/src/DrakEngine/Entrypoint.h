#pragma once

extern DrakEngine::Application* DrakEngine::CreateApplication();

int main(int argc, char *argv[]) {
    auto app = DrakEngine::CreateApplication();
    app->Run();
    return 0;
}
