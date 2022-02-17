#pragma once

#include <memory>
#include "DrakEngine/Renderer/Buffer.h"

namespace DrakEngine {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffers() const = 0;

        static Ref<VertexArray> Create();
    };
}
