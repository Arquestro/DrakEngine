#include "dkpch.h"
#include "DrakEngine/Renderer/RendererAPI.h"

#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace DrakEngine {
    // Define API here
    // TODO: make macros based definition
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;

    Scope<RendererAPI> RendererAPI::Create() {
        switch (s_API) {
            case API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
            case API::Vulkan: return CreateScope<VulkanRendererAPI>();
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }
}
