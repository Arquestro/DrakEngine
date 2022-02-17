#pragma once

#include "DrakEngine/Core/Layer.h"
#include "DrakEngine/ImGui/ImGuiVulkanImplAPI.h"
#include "backends/imgui_impl_vulkan.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace DrakEngine {


    class ImGuiLayerVulkan final : public Layer {
    public:
        ImGuiLayerVulkan();
        ~ImGuiLayerVulkan() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void SetDarkThemeColors();
    private:
        void SetupVulkan(std::vector<const char*>* instance_extensions, std::vector<const char*>* device_extensions);

        bool m_BlockEvents = true;
        Scope<ImGuiVulkanImplAPI> m_VulkanImGuiRenderer;
    };
}
