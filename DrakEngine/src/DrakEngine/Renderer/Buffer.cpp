#include "dkpch.h"
#include "DrakEngine/Renderer/Buffer.h"
#include "DrakEngine/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace DrakEngine {
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
//            case RendererAPI::API::Vulkan: return CreateRef<VulkanVertexBuffer>(size);
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
//            case RendererAPI::API::Vulkan: return CreateRef<VulkanVertexBuffer>(vertices, size);
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
//            case RendererAPI::API::Vulkan: return CreateRef<VulkanIndexBuffer>(indices, size);
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }
}
