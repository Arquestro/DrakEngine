#pragma once

#include "DrakEngine/Core.h"
#include "DrakEngine/Core/Layer.h"

#include <vector>

namespace DrakEngine {
    class LayerStack {
    public:
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<Layer*> m_Layers;
        uint32_t m_LayerInsert = 0;
    };
}
