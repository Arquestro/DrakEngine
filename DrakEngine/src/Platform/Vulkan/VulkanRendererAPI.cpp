#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanValidationLayers.h"
#include "Platform/Vulkan/VulkanUtilities.h"

#include <algorithm>


namespace DrakEngine {
    VulkanRendererAPI::~VulkanRendererAPI() {
        vkDestroyDevice(m_MainDevice.m_LogicalDevice, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    };

    void VulkanRendererAPI::Init() {
        try {
            CreateInstance();
            FindAndSetPhysicalDevice();
            CreateLogicalDevice();
        } catch(const std::runtime_error &e) {
            DRAK_CORE_ERROR("ERROR: ", e.what());
        }
    }

    void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
// TODO: Implement
    }

    void VulkanRendererAPI::SetClearColor(const glm::vec4& color) {
        m_ClearColor = color;
    }

    void VulkanRendererAPI::Clear() {
// TODO: Implement
    }

    void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count) {
// TODO: Implement
    }

    void VulkanRendererAPI::CreateInstance() {
        if (g_EnableValidationLayers && !CheckValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but same of them are not available!");
        }
        // Information about the application itself
        VkApplicationInfo application_info = {};
        {
            application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            application_info.pApplicationName = "Vulkan App";                // Custom name of the applicaiton
            application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);  // Custom version of the application
            application_info.pEngineName = "No Engine";                      // Custom engine name
            application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);       // Custom engine version
            application_info.apiVersion = VK_API_VERSION_1_0;                // Api version
        }
        // Creation of information for a VkInstance
        VkInstanceCreateInfo create_info = {};
        {
            create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  // To implement fast reflection-like
            create_info.pApplicationInfo = &application_info;
            if (g_EnableValidationLayers) {
                create_info.enabledLayerCount = static_cast<uint32_t>(g_ValidationLayers.size());
                create_info.ppEnabledLayerNames = g_ValidationLayers.data();
                m_InstanceExtensions.push_back("VK_EXT_debug_report");
            } else {
                create_info.enabledLayerCount = 0;
                create_info.ppEnabledLayerNames = nullptr;
            }
            create_info.enabledExtensionCount = static_cast<uint32_t>(m_InstanceExtensions.size());
            create_info.ppEnabledExtensionNames = m_InstanceExtensions.data();
            // Check Instance Extensions supported...
            if (!CheckInstanceExtensionSupport(&m_InstanceExtensions)) {
                throw std::runtime_error("VkInstance does not support required extensions!");
            }
        }
        // Create m_Instance
        VkResult result = vkCreateInstance(&create_info, nullptr, &m_Instance);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a vulkan m_Instance!");
        }
    }

    void VulkanRendererAPI::CreateLogicalDevice() {
        // Get the queue family indices for the chosen Physical Device
        QueueFamilyIndices indices = GetQueueFamilies(m_MainDevice.m_PhysicalDevice);
        // Queue the logical device needs to create and info to do so (Only 1 for now, will add more later!)
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = indices.m_GraphicsFamily;          // The index of the family to create a queue from
        queue_create_info.queueCount = 1;                                       // Number of queues to create
        float priority = 1.0f;
        queue_create_info.pQueuePriorities = &priority;                         // Vulkan needs to know how to handle multiple queues
        // Information to create logical device(sometimes called "device")
        VkDeviceCreateInfo device_create_info = {};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.queueCreateInfoCount = 1;                             // Number of Queue Create Infos
        device_create_info.pQueueCreateInfos = &queue_create_info;               // List of queue create infos so device can create required queues
        AddDeviceExtensionName("VK_KHR_portability_subset");
        const char** logical_device_extension_names = m_DeviceExtensions.data();
        // Check Device Extensions supported...
        if (!CheckDeviceExtensionSupport(&m_DeviceExtensions)) {
            throw std::runtime_error("VkDevice does not support required extensions!");
        }
        device_create_info.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());     // Number of enabled logical device extensions
        device_create_info.ppEnabledExtensionNames = logical_device_extension_names;                    // List of enabled logical device extensions
        // Physical Device Features the Logical Device will be using
        VkPhysicalDeviceFeatures device_features = {};
        device_create_info.pEnabledFeatures = &device_features;                                         // Physical Device features Logical Device will use
        // Create the logical device for the given physical device
        VkResult result = vkCreateDevice(
            m_MainDevice.m_PhysicalDevice,
            &device_create_info,
            nullptr,
            &m_MainDevice.m_LogicalDevice
            );
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Logical Device!");
        }
        // Queues are created at the same time as the device...
        // So we want handle to queues
        // From  given logical device, of given Queue Family, of given Queue Index (0, since only one queue), place reference in given VkQueue
        vkGetDeviceQueue(
            m_MainDevice.m_LogicalDevice,
            indices.m_GraphicsFamily,
            0,
            &m_GraphicsQueue
            );

    }

    void VulkanRendererAPI::FindAndSetPhysicalDevice() {
        // Enumerate Physical devices the vkInstance can access
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_Instance, &device_count, nullptr);
        // If no devices available, then none support Vulkan!
        if(device_count == 0) {
            throw std::runtime_error("Can't find GPUs that support Vulkan Instance!");
        }
        // Get list of Physical Devices
        std::vector<VkPhysicalDevice> device_list(device_count);
        vkEnumeratePhysicalDevices(m_Instance, &device_count, device_list.data());
        for(const auto &device: device_list) {
            if(CheckDeviceSuitable(device)) {
                m_MainDevice.m_PhysicalDevice = device;
                break;
            }
        }
    }

    bool VulkanRendererAPI::CheckInstanceExtensionSupport(std::vector<const char *> *check_extensions) {
        // Need to get number of extensions to create array of correct size to hold extensions
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        // Create a list of VkExtensionProperties using count
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
        // Check if given extensions are in list of available extensions
        for(const auto &check_extension: *check_extensions) {
            bool has_extension = false;
            for(const auto &extension: extensions) {
                if(strcmp(check_extension, extension.extensionName) == 0) {
                    has_extension = true;
                    break;
                }
            }
            if(!has_extension) {
                return false;
            }
        }
        return true;
    }

    bool VulkanRendererAPI::CheckDeviceExtensionSupport(std::vector<const char *> *check_extensions) const {
        // Need to get number of extensions to create array of correct size to hold extensions
        uint32_t extension_count = 0;
        vkEnumerateDeviceExtensionProperties(
            m_MainDevice.m_PhysicalDevice,
            nullptr,
            &extension_count,
            nullptr
            );
        // Create a list of VkExtensionProperties using count
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(
            m_MainDevice.m_PhysicalDevice,
            nullptr,
            &extension_count,
            extensions.data()
            );
        // Check if given extensions are in list of available extensions
        for(const auto &check_extension: *check_extensions) {
            bool has_extension = false;
            for(const auto &extension: extensions) {
                if(strcmp(check_extension, extension.extensionName) == 0) {
                    has_extension = true;
                    break;
                }
            }
            if(!has_extension) {
                return false;
            }
        }
        return true;
    }

    bool VulkanRendererAPI::CheckDeviceSuitable(VkPhysicalDevice device) {
        // Information about the device itself (ID, name, type, vendor, etc)
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        // Information about what the device can do (geo shader, tesselation shader, wide, lines, etc.)
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(device, &device_features);
        QueueFamilyIndices indices = GetQueueFamilies(device);
        return indices.IsValid();
    }

    QueueFamilyIndices VulkanRendererAPI::GetQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;
        // Get all Queue Family Property info for the given device
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            device,
            &queue_family_count,
            nullptr
            );
        std::vector<VkQueueFamilyProperties> queue_family_list(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(
            device,
            &queue_family_count,
            queue_family_list.data()
            );
        // Go through each queue family and check if it has at least 1 of the required types of queue
        int i = 0;
        for(const auto &queue_family : queue_family_list) {
            // First check if queue family has at least 1 queue in that family (could have no queues)
            // Queue can be multiple types deefined through bitfield.
            // Need to bitwise AND with VK_QUEUE_*_BIT to check it has required type
            if(queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.m_GraphicsFamily = i; // If queue family is valid, then get index
            }
            // Check if queue family indices are in a valid state, stop searching if so
            if(indices.IsValid()) {
                break;
            }
            i++;
        }
        return indices;
    }

    void VulkanRendererAPI::AddInstanceExtensionName(const char *extension_name) {
        auto find_extension_name_res = std::find_if(
            m_InstanceExtensions.begin(),
            m_InstanceExtensions.end(),
            [=](const char *x) { return std::strcmp(x, extension_name) == 0; }
            );
        if (find_extension_name_res == m_InstanceExtensions.end()) {
            m_InstanceExtensions.push_back(extension_name);
        } else {
            DRAK_CORE_WARN("Instance extension \"" + std::string(extension_name) + "\" already added.");
        }
    }

    void VulkanRendererAPI::AddInstanceExtensionNames(std::vector<const char *> *extension_names) {
        for (const auto& extension: *extension_names) {
            AddInstanceExtensionName(extension);
        }
    }

    void VulkanRendererAPI::AddDeviceExtensionName(const char *extension_name) {
        auto find_extension_name_res = std::find_if(
            m_DeviceExtensions.begin(),
            m_DeviceExtensions.end(),
            [=](const char *x) { return std::strcmp(x, extension_name) == 0; }
        );
        if (find_extension_name_res == m_DeviceExtensions.end()) {
            m_DeviceExtensions.push_back(extension_name);
        } else {
            DRAK_CORE_WARN("Device extension \"" + std::string(extension_name) + "\" already added.");
        }
    }

    void VulkanRendererAPI::AddDeviceExtensionNames(std::vector<const char *> *extension_names) {
        for (const auto& extension: *extension_names) {
            AddDeviceExtensionName(extension);
        }
    }
}
