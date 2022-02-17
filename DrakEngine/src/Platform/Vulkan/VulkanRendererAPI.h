#pragma once

#include "DrakEngine/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanUtilities.h"

#include <vulkan/vulkan.h>

namespace DrakEngine {
    class VulkanRendererAPI : public RendererAPI {
    public:
        virtual ~VulkanRendererAPI();

        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count) override;

        void AddInstanceExtensionName(const char* extension_name);
        void AddInstanceExtensionNames(std::vector<const char*>* extension_names);
        void AddDeviceExtensionName(const char* extension_name);
        void AddDeviceExtensionNames(std::vector<const char*>* extension_names);
    protected:
        void CreateInstance();
        void CreateLogicalDevice();

        void FindAndSetPhysicalDevice();

        static bool CheckInstanceExtensionSupport(std::vector<const char*>* check_extensions);
        bool CheckDeviceExtensionSupport(std::vector<const char*>* check_extensions) const;
        bool CheckDeviceSuitable(VkPhysicalDevice device);

        QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device);
    protected:
        VkInstance m_Instance;
        struct VulkanMainDevice {
            VkPhysicalDevice m_PhysicalDevice;
            VkDevice m_LogicalDevice;
        } m_MainDevice;
        VkQueue m_GraphicsQueue;
        std::vector<const char*> m_InstanceExtensions;
        std::vector<const char*> m_DeviceExtensions;
    };
}
