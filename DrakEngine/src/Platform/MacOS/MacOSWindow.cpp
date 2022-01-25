#include "dkpch.h"
#include "Platform/MacOS/MacOSWindow.h"

#include "DrakEngine/Events/ApplicationEvent.h"
#include "DrakEngine/Events/MouseEvent.h"
#include "DrakEngine/Events/KeyEvent.h"

namespace DrakEngine {
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char *description) {
        DRAK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    MacOSWindow::MacOSWindow(const WindowProps &props) {
        Init(props);
    }

    MacOSWindow::~MacOSWindow() {
        Shutdown();
    }

    void MacOSWindow::Init(const WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        DRAK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (s_GLFWWindowCount == 0) {
            int success = glfwInit();
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        {
            m_Window = glfwCreateWindow((int) props.Width, (int) props.Height, m_Data.Title.c_str(), nullptr, nullptr);
            ++s_GLFWWindowCount;
        }

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(false);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
            WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float) xPos, (float) yPos);
            data.EventCallback(event);
        });
    }

    void MacOSWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0) {
            glfwTerminate();
        }
    }

    void MacOSWindow::OnUpdate() {
        glfwPollEvents();
    }

    void MacOSWindow::SetVSync(bool enabled) {
        //if (enabled) {
        //    glfwSwapInterval(1);
        //} else {
        //    glfwSwapInterval(0);
        //}

        m_Data.VSync = enabled;
    }

    bool MacOSWindow::IsVSync() const {
        return m_Data.VSync;
    }
}
