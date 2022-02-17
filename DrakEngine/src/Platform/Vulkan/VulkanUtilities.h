#pragma once

#include <vulkan/vulkan.h>

namespace DrakEngine {
    // Indices (locations) of Queue Families (if they exist at all)
    struct QueueFamilyIndices {
        int m_GraphicsFamily = -1;

        // Check if Queue Families are valid
        bool IsValid() {
            return m_GraphicsFamily >= 0;
        }
    };

    inline void CheckVkResult(VkResult result, std::string message) {
        if (result != VK_SUCCESS) {
            message += "VkResult: " + std::to_string(result);
            DRAK_ERROR(message);
            throw std::runtime_error(message);
        }
    }

    inline void CheckVkResult(VkResult result) {
        CheckVkResult(result, "");
    }
}
