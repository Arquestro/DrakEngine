// TODO: Implement Vulkan Buffers
#pragma once

#include "DrakEngine/Renderer/Buffer.h"

namespace DrakEngine {
    class VulkanVertexBuffer : public VertexBuffer {
    public:
        void Bind() const override {

        }
        void Unbind() const override {

        }
        void SetData(const void *data, uint32_t size) override {

        }
        const BufferLayout &GetLayout() const override {
            return std::move(BufferLayout());
        }
        void SetLayout(const BufferLayout &layout) override {

        }
    };

    class VulkanIndexBuffer : public IndexBuffer {
    public:
        void Bind() const override {

        }
        void Unbind() const override {

        }
        uint32_t GetCount() const override {
            return 0;
        }
    };
}