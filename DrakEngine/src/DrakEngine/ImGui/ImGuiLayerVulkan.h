#pragma once

#include "DrakEngine/ImGui/ImGuiLayer.h"
#include "DrakEngine/Core/Layer.h"
#include "DrakEngine/ImGui/ImGuiVulkanImplAPI.h"
#include "backends/imgui_impl_vulkan.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace DrakEngine {
    class ImGuiLayerVulkan final : public ImGuiLayer {
    public:
        ImGuiLayerVulkan() : ImGuiLayer("ImGuiLayerVulkan") {}
        virtual ~ImGuiLayerVulkan() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void SetDarkThemeColors();
    private:
        void SetupVulkan(std::vector<const char*>* instance_extensions, std::vector<const char*>* device_extensions);

        Scope<ImGuiVulkanImplAPI> m_VulkanImGuiRenderer;
    };
}
