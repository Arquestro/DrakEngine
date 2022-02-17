#include "dkpch.h"
#include "DrakEngine/Renderer/Renderer.h"

namespace DrakEngine {
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
    
    void Renderer::Init() {
        RenderCommand::Init();
    }
    
    void Renderer::Shutdown() {}

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera) {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform) {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transfrom", transform);

        vertex_array->Bind();
        RenderCommand::DrawIndexed(vertex_array);
    }
}
