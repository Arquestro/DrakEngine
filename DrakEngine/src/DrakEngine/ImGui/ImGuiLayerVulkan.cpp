#include "dkpch.h"
#include "DrakEngine/ImGui/ImGuiLayerVulkan.h"
#include "DrakEngine/Application.h"

#include <imgui.h>

namespace DrakEngine {
    void ImGuiLayerVulkan::SetupVulkan(std::vector<const char*>* instance_extensions, std::vector<const char*>* device_extensions) {
        m_VulkanImGuiRenderer = CreateScope<ImGuiVulkanImplAPI>();
        m_VulkanImGuiRenderer->AddInstanceExtensionNames(instance_extensions);
        m_VulkanImGuiRenderer->AddDeviceExtensionNames(device_extensions);
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        m_VulkanImGuiRenderer->Init(window);
        m_VulkanImGuiRenderer->SetClearColor({0.45f, 0.55f, 0.60f, 1.00f});
    }

    void ImGuiLayerVulkan::OnAttach() {
        // Setup GLFW window
        Application& app = Application::Get();
        auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        // Setup Vulkan
        if (!glfwVulkanSupported()) {
            DRAK_CORE_CRITICAL("GLFW: Vulkan Not Supported\n");
            return;
        }
        // Define instance extensions
        uint32_t instance_extensions_count = 0;
        std::vector<const char*> instance_extensions;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&instance_extensions_count);
        for (int i = 0; i < instance_extensions_count; i++) {
            instance_extensions.push_back(glfw_extensions[i]);
        }
#ifdef DRAK_PLATFORM_MACOS
        instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif
        // Define device extensions
        std::vector<const char*> device_extensions;
        device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        ImGui::CreateContext();
        SetupVulkan(&instance_extensions, &device_extensions);
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // Set Dear ImGui style
        ImGui::StyleColorsDark();
        SetDarkThemeColors();
    }

    void ImGuiLayerVulkan::OnDetach() {
        // Cleanup
        m_VulkanImGuiRenderer->DeviceWaitIdle();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_VulkanImGuiRenderer.reset(nullptr);
    }

    void ImGuiLayerVulkan::OnEvent(Event& event) {
        if (m_BlockEvents) {
            ImGuiIO& io = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayerVulkan::OnUpdate() {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        m_VulkanImGuiRenderer->OnUpdate(window);
        ImGui::NewFrame();
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        m_VulkanImGuiRenderer->RenderImDrawData(draw_data);
    }

    void ImGuiLayerVulkan::SetDarkThemeColors() {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }
}
