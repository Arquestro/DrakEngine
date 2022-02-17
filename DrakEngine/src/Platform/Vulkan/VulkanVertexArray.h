// TODO: Implement Vulkan Vertex Array
#pragma once

#include "DrakEngine/Renderer/VertexArray.h"

namespace DrakEngine {
    class VulkanVertexArray : public VertexArray {
    public:
        void Bind() const override {

        }
        void Unbind() const override {

        }
        void AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) override {

        }
        void SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) override {

        }
        const std::vector<Ref < VertexBuffer>> &
        GetVertexBuffers() const override {
            return std::vector<Ref < VertexBuffer>>();
        }
        const Ref<IndexBuffer> &GetIndexBuffers() const override {
            return nullptr;
        }
    };
}