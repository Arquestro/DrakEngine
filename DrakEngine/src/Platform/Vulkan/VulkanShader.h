// TODO: Implement Vulkan Shaders
#pragma once

#include "DrakEngine/Renderer/Shader.h"

namespace DrakEngine {
    class VulkanShader : public Shader {
    public:
        void Bind() const override {

        }
        void Unbind() const override {

        }
        void SetInt(const std::string &name, int value) override {

        }
        void SetIntArray(const std::string &name, int *values, uint32_t count) override {

        }
        void SetFloat(const std::string &name, float value) override {

        }
        void SetFloat2(const std::string &name, const glm::vec2 &value) override {

        }
        void SetFloat3(const std::string &name, const glm::vec3 &value) override {

        }
        void SetFloat4(const std::string &name, const glm::vec4 &value) override {

        }
        void SetMat4(const std::string &name, const glm::mat4 &value) override {

        }
        const std::string &GetName() const override {
            return "";
        }
    };
}