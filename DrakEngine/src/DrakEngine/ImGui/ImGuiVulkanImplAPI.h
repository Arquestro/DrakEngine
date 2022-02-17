#pragma once

#include "Platform/Vulkan/VulkanRendererAPI.h"

#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

namespace DrakEngine {
    class ImGuiVulkanImplAPI final : public VulkanRendererAPI {
    public:
        ~ImGuiVulkanImplAPI() override;

        void Init(GLFWwindow *window);
        void OnUpdate(GLFWwindow* window);
        void DeviceWaitIdle();
        void RenderImDrawData(ImDrawData* draw_data);
    private:
        void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
        void FramePresent(ImGui_ImplVulkanH_Window* wd);
        void CreateDescriptorPool();
        void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

        VkAllocationCallbacks* m_Allocator;
        VkDescriptorPool m_DescriptorPool;
        ImGui_ImplVulkanH_Window m_MainWindowData;
        bool m_SwapChainRebuild = false;
        uint32_t m_QueueFamily = -1;
        int m_MinImageCount = 2;
        VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;
    };
}
