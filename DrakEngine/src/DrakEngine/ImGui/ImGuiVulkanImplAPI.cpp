#include "DrakEngine/ImGui/ImGuiVulkanImplAPI.h"
#include "Platform/Vulkan/VulkanUtilities.h"

namespace DrakEngine {
    ImGuiVulkanImplAPI::~ImGuiVulkanImplAPI() {
        ImGui_ImplVulkanH_DestroyWindow(m_Instance, m_MainDevice.m_LogicalDevice, &m_MainWindowData, m_Allocator);
        vkDestroyDescriptorPool(m_MainDevice.m_LogicalDevice, m_DescriptorPool, m_Allocator);
        VulkanRendererAPI::~VulkanRendererAPI();
    }

    void ImGuiVulkanImplAPI::CreateDescriptorPool() {
        VkDescriptorPoolSize pool_sizes[] =
            {
                { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
            };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        auto err = vkCreateDescriptorPool(
            m_MainDevice.m_LogicalDevice,
            &pool_info,
            m_Allocator,
            &m_DescriptorPool
        );
        CheckVkResult(err, "Could not create descriptor pool!");
    }

    void ImGuiVulkanImplAPI::Init(GLFWwindow *window) {
        VulkanRendererAPI::Init();
        QueueFamilyIndices indices = GetQueueFamilies(m_MainDevice.m_PhysicalDevice);
        m_QueueFamily = indices.m_GraphicsFamily;
        CreateDescriptorPool();
        // Create Window Surface
        VkSurfaceKHR surface;
        VkResult err = glfwCreateWindowSurface(m_Instance, window, m_Allocator, &surface);
        CheckVkResult(err, "Could not create glfw window surface!");
        // Create Framebuffers
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        auto wd = &m_MainWindowData;
        SetupVulkanWindow(wd, surface, w, h);
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_Instance;
        init_info.PhysicalDevice = m_MainDevice.m_PhysicalDevice;
        init_info.Device = m_MainDevice.m_LogicalDevice;
        init_info.QueueFamily = m_QueueFamily;
        init_info.Queue = m_GraphicsQueue;
        init_info.PipelineCache = m_PipelineCache;
        init_info.DescriptorPool = m_DescriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = m_MinImageCount;
        init_info.ImageCount = wd->ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = m_Allocator;
        init_info.CheckVkResultFn = CheckVkResult;
        ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
        // Upload Fonts
        {
            // Use any command queue
            VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
            VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;
            err = vkResetCommandPool(m_MainDevice.m_LogicalDevice, command_pool, 0);
            CheckVkResult(err);
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(command_buffer, &begin_info);
            CheckVkResult(err);
            ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &command_buffer;
            err = vkEndCommandBuffer(command_buffer);
            CheckVkResult(err);
            err = vkQueueSubmit(m_GraphicsQueue, 1, &end_info, VK_NULL_HANDLE);
            CheckVkResult(err);
            err = vkDeviceWaitIdle(m_MainDevice.m_LogicalDevice);
            CheckVkResult(err);
            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
    }

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
    void ImGuiVulkanImplAPI::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height) {
        wd->Surface = surface;
        // Check for WSI support
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            m_MainDevice.m_PhysicalDevice,
            m_QueueFamily,
            wd->Surface,
            &res
        );
        if (res != VK_TRUE) {
            std::string err_msg = "Error no WSI support on physical device 0";
            DRAK_ERROR(err_msg);
            throw std::runtime_error(err_msg);
        }
        // Select Surface Format
        const VkFormat requestSurfaceImageFormat[] = {
            VK_FORMAT_B8G8R8A8_UNORM,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_FORMAT_B8G8R8_UNORM,
            VK_FORMAT_R8G8B8_UNORM
        };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
            m_MainDevice.m_PhysicalDevice,
            wd->Surface,
            requestSurfaceImageFormat,
            (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
            requestSurfaceColorSpace
        );
        // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
            m_MainDevice.m_PhysicalDevice,
            wd->Surface,
            &present_modes[0],
            IM_ARRAYSIZE(present_modes)
        );
        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(m_MinImageCount >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(
            m_Instance,
            m_MainDevice.m_PhysicalDevice,
            m_MainDevice.m_LogicalDevice,
            wd,
            m_QueueFamily,
            m_Allocator,
            width,
            height,
            m_MinImageCount
        );
    }

    void ImGuiVulkanImplAPI::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
    {
        VkResult err;
        VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        err = vkAcquireNextImageKHR(
            m_MainDevice.m_LogicalDevice,
            wd->Swapchain,
            UINT64_MAX,
            image_acquired_semaphore,
            VK_NULL_HANDLE,
            &wd->FrameIndex
        );
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            m_SwapChainRebuild = true;
            return;
        }
        CheckVkResult(err);
        ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
        {
            err = vkWaitForFences(m_MainDevice.m_LogicalDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
            CheckVkResult(err);
            err = vkResetFences(m_MainDevice.m_LogicalDevice, 1, &fd->Fence);
            CheckVkResult(err);
        }
        {
            err = vkResetCommandPool(m_MainDevice.m_LogicalDevice, fd->CommandPool, 0);
            CheckVkResult(err);
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            CheckVkResult(err);
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = wd->RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = 1;
            info.pClearValues = &wd->ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }
        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &image_acquired_semaphore;
            info.pWaitDstStageMask = &wait_stage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &render_complete_semaphore;
            err = vkEndCommandBuffer(fd->CommandBuffer);
            CheckVkResult(err);
            err = vkQueueSubmit(m_GraphicsQueue, 1, &info, fd->Fence);
            CheckVkResult(err);
        }
    }

    void ImGuiVulkanImplAPI::FramePresent(ImGui_ImplVulkanH_Window* wd) {
        if (m_SwapChainRebuild)
            return;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &wd->Swapchain;
        info.pImageIndices = &wd->FrameIndex;
        VkResult err = vkQueuePresentKHR(m_GraphicsQueue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
            m_SwapChainRebuild = true;
            return;
        }
        CheckVkResult(err);
        wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
    }

    void ImGuiVulkanImplAPI::OnUpdate(GLFWwindow *window) {
        // Resize swap chain?
        if (m_SwapChainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(m_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(
                    m_Instance,
                    m_MainDevice.m_PhysicalDevice,
                    m_MainDevice.m_LogicalDevice,
                    &m_MainWindowData,
                    m_QueueFamily,
                    m_Allocator,
                    width,
                    height,
                    m_MinImageCount
                );
                m_MainWindowData.FrameIndex = 0;
                m_SwapChainRebuild = false;
            }
        }
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    void ImGuiVulkanImplAPI::DeviceWaitIdle() {
        auto err = vkDeviceWaitIdle(m_MainDevice.m_LogicalDevice);
        CheckVkResult(err);
    }

    void ImGuiVulkanImplAPI::RenderImDrawData(ImDrawData *draw_data) {
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            auto wd = &m_MainWindowData;
            ImVec4 clear_color = ImVec4(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
            wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
            wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
            wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
            wd->ClearValue.color.float32[3] = clear_color.w;
            FrameRender(wd, draw_data);
            FramePresent(wd);
        }
    }
}