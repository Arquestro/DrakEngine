#pragma once

#include "DrakEngine/Core/Logger.h"

extern DrakEngine::Application* DrakEngine::CreateApplication();

int main(int argc, char *argv[]) {
    DrakEngine::Logger::Init();
    auto app = DrakEngine::CreateApplication();
    app->Run();
    return 0;
}
