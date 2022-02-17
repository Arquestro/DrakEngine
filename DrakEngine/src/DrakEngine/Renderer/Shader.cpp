#include "dkpch.h"
#include "DrakEngine/Renderer/Shader.h"
#include "DrakEngine/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanShader.h"

namespace DrakEngine {
    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
//            case RendererAPI::API::Vulkan: return CreateRef<VulkanShader>(filepath);
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: DRAK_CORE_CRITICAL("RendererAPI::None is currently not supported!"); return nullptr;
//            case RendererAPI::API::Vulkan: return CreateRef<VulkanShader>(name, vertex_src, fragment_src);
        }

        DRAK_CORE_CRITICAL("Unknown RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
        if(!Exists(name)) DRAK_CORE_WARN("Shader \"" + name + "\" already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader) {
        auto& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name) {
        if(Exists(name)) DRAK_CORE_WARN("Shader \"" + name + "\" not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}
