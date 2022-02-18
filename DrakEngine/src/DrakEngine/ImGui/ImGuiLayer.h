#pragma once

#include "DrakEngine/Core/Layer.h"

namespace DrakEngine {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer() : Layer("ImGuiLayer") {}
        ImGuiLayer(std::string name) : Layer(name) {}
        virtual ~ImGuiLayer() = default;

        virtual void BlockEvents(bool block) = 0;

        virtual void SetDarkThemeColors() = 0;
    protected:
        bool m_BlockEvents = true;
    };
}
