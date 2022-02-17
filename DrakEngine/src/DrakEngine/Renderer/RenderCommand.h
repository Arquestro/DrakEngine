#pragma once

#include "DrakEngine/Renderer/RendererAPI.h"

namespace DrakEngine {
    class RenderCommand {
    public:
        static void Init() {
            s_Renderer_API->Init();
        }

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            s_Renderer_API->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color) {
            s_Renderer_API->SetClearColor(color);
        }

        static void Clear() {
            s_Renderer_API->Clear();
        }

        static void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t count = 0) {
            s_Renderer_API->DrawIndexed(vertex_array, count);
        }
    private:
        static Scope<RendererAPI> s_Renderer_API;
    };
}
