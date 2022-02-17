#include "dkpch.h"
#include "DrakEngine/Renderer/VertexArray.h"

#include "DrakEngine/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanVertexArray.h"

namespace DrakEngine {
    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
//            case RendererAPI::API::Vulkan: return CreateScope<VulkanVertexArray>();
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }
}
