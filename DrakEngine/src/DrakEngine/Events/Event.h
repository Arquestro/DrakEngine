#pragma once

#include <string>
#include <ostream>
#include <type_traits>

#include "DrakEngine/Core.h"

namespace DrakEngine {

    // Events in DrakEngine are currently the same as in 
    // Hazel(https://github.com/TheCherno/Hazel/blob/f627b9c90923382f735350cd3060892bbd4b1e75/Hazel/src/Hazel/Events/Event.h)
    // So, they're blocking and maybe there is a point in making them buffered and processed during update

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // EventCategory enum should be defined with bit masks to enable a combination of categories
    enum EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event {
    public:
        virtual ~Event() = default;

        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category) {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher {
    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        // F should be deduced by the compiler
        template<typename T, typename F>
        bool Dispatch(const F& func) {
            if(m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }
}
